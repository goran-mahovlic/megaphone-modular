#include "includes.h"
#include "records.h"

// We sort 160 records = 80KB = 1/10th of a disk image at a time.
#define MAX_RECORDS 160
#define STACK_SIZE  160  // safe headroom

typedef struct {
    unsigned char low;
    unsigned char high;
} partition_t;

unsigned char indices[MAX_RECORDS];  // indices into your slab records
partition_t stack[STACK_SIZE];
int stack_ptr = 0;


unsigned char rec_a[RECORD_DATA_SIZE];
unsigned char rec_b[RECORD_DATA_SIZE];
unsigned char idx_b = 0xff;

int compare_records(unsigned char a_idx, unsigned char b_idx, unsigned char field_id) {
  unsigned int len_a = 0, len_b = 0, l;
  unsigned long addr;
  unsigned char *field_a, *field_b;
  
  if (b_idx != idx_b) {
    // B record cache invalid so desectorise from high RAM
    addr = WORK_BUFFER_ADDRESS + (((unsigned long)b_idx)<<9);
    lcopy(addr + 2,(unsigned long)&rec_b[0],254);
    lcopy(addr + 256+2,(unsigned long)&rec_b[254],254);      
  }
  
  // Always fetch record a
  addr = WORK_BUFFER_ADDRESS + (((unsigned long)a_idx)<<9);
  lcopy(addr + 2,(unsigned long)&rec_a[0],254);
  lcopy(addr + 256+2,(unsigned long)&rec_a[254],254);      
  
  // char *find_field(char *record, unsigned int bytes_used, unsigned char type, unsigned int *len);
  field_a = find_field(rec_a, RECORD_DATA_SIZE, field_id, &len_a);
  field_b = find_field(rec_b, RECORD_DATA_SIZE, field_id, &len_b);
  
  int min_len = (len_a < len_b) ? len_a : len_b;
  char cmp=0;
  for(l=0;l<min_len;l++) {
    if (field_a[l]<field_b[l]) { cmp=-1; break;}
    if (field_a[l]>field_b[l]) { cmp=1; break;}
  }
  
  if (cmp == 0) {
    return (int)len_a - (int)len_b; // shorter field sorts first
  }
  return cmp;
}



void quicksort_indices(unsigned char field_id, unsigned char count) {
    // Initialize stack with full range
    stack[0].low = 0;
    stack[0].high = count - 1;
    stack_ptr = 1;

    while (stack_ptr > 0) {
        stack_ptr--;
        unsigned char low  = stack[stack_ptr].low;
        unsigned char high = stack[stack_ptr].high;

        if (low >= high) continue;

        // Choose pivot = high
        unsigned char pivot_idx = indices[high];
        unsigned char i = low;

        for (unsigned char j = low; j < high; j++) {
            if (compare_records(indices[j], pivot_idx, field_id) <= 0) {
                // Swap indices[i] and indices[j]
                unsigned char tmp = indices[i];
                indices[i] = indices[j];
                indices[j] = tmp;
                i++;
            }
        }

        // Move pivot into correct location
        unsigned char tmp = indices[i];
        indices[i] = indices[high];
        indices[high] = tmp;

        // Push sub-partitions onto stack
        if (i > low + 1) {
            stack[stack_ptr].low = low;
            stack[stack_ptr].high = i - 1;
            stack_ptr++;
        }
        if (i + 1 < high) {
            stack[stack_ptr].low = i + 1;
            stack[stack_ptr].high = high;
            stack_ptr++;
        }
    }
}

void sort_slab(unsigned char field_id)
{
  /*
    Do a non-recursive quicksort of the 80KB slab of sectors
    from drive 0, using drive 1 as temporary store
   */
  unsigned char c;
  // Set up ordinal record numbers
  for(c=0;c<MAX_RECORDS;c++) indices[c]=c;
  // invalidate record cache for idx_b, which is repeatedly used in quicksort.
  // This saves us from copying down records that we already have buffered.
  idx_b=0xff; 
  // Sort the list of indices
  quicksort_indices(field_id, MAX_RECORDS);
  // Write the sorted indices out to the temporary D81 in drive 1
}

#define NUM_SLABS (80/8)
unsigned char cached_track[NUM_SLABS];
unsigned char cached_track_stop[NUM_SLABS];
unsigned char cached_next_sector[NUM_SLABS];
unsigned char next_slab, next_slab_sector;  

char sort_d81(char *name_in, char *name_out, unsigned char field_id)
{
  unsigned char out_track=1;
  unsigned char out_sector=0;
  unsigned char slab=0;
  unsigned char s;
  
  // Mount D81 to be sorted in drive 0
  if (mount_d81(name_in,0)) return 7;
  
  // Mount a scratch D81 in drive 1  
  if (mount_d81("SCRATCH.D81",1)) return 8;

  // fprintf(stderr,"DEBUG: sort_d81(): Sort each slab.\n");
  
  // Do internal sort of each 80KB slab of the disk.
  for(slab=0;slab<10;slab++) {
    // Read in 80KB = 8 tracks of data at 0x40000
    unsigned char t=1+(slab<<3);
    unsigned char t_stop=t+8;
    unsigned long rec_addr = WORK_BUFFER_ADDRESS;
    unsigned char n=0;

    // fprintf(stderr,"DEBUG: sort_d81(): Sort slab %d.\n",slab);
    
    // Load the slab into RAM
    for(;t<t_stop;t++) {
      for(s=0;s<20;s++) {
	if (read_sector(0,t,s)) return 1;
	// Read sector, or if it's track 40, pretend it's empty, so that we don't
	// include CBM DOS BAM or directory sectors in the sort.
	if (t!=40) lcopy((unsigned long)SECTOR_BUFFER_ADDRESS,rec_addr,512);
	else lfill((unsigned long)SECTOR_BUFFER_ADDRESS,0x00,512);

	rec_addr+=512;
      }
    }

    // Get a sorted list of indices
    sort_slab(field_id);

    // Write out the sorted sectors into the scratch disk image
    t=1+(slab<<3);
    for(;t<t_stop;t++) {
      for(s=0;s<20;s++) {
	rec_addr = WORK_BUFFER_ADDRESS + (((unsigned long)indices[n++])<<9);
	lcopy(rec_addr,(unsigned long)SECTOR_BUFFER_ADDRESS,512);
	if (write_sector(1,t,s)) return 2;
      }
    }
    
  }

  // Now we have the slabs sorted, we need to do an external merge of those slabs back into the 
  // sorted output D81.
  // We don't care about the sector links in the sorted disk images, because these images are
  // generated on demand. If someone validates a sorted D81, the result won't be tragic, since
  // we can just regenerate it.

  // Mount output D81 as drive 0
  if (mount_d81(name_out,0)) return 9;

  fprintf(stderr,"DEBUG: sort_d81(): Read merge cache.\n");
  
  // Prime cache of sectors from each slab. We have 128KB at WORK_BUFFER_ADDRESS we can use.
  // So we will make each cache be a whole track.
  for(slab=0;slab<10;slab++) {
    cached_track[slab]=(slab<<3)+1;
    cached_track_stop[slab]=(slab<<3)+1+8;
    cached_next_sector[slab]=0;

    // fprintf(stderr,"DEBUG: Reading track %d for slab %d\n",cached_track[slab],slab);
    
    for(s=0;s<20;s++) {
      if (read_sector(1,cached_track[slab],s)) return 3;
      lcopy((unsigned long)SECTOR_BUFFER_ADDRESS,WORK_BUFFER_ADDRESS + slab*(512L*20) + s*512L, 512);
    }
  }

  // Now do round-robin comparisons of the next sector in each cached track to find
  // the next sector to write out to the disk image.
  // Stop when no more sectors to check.
  do {
    next_slab=0xff;
    next_slab_sector = 0;

    for(slab=0;slab<10;slab++) {
      if (cached_next_sector[slab]==20) continue;
      if (next_slab==0xff) {
	next_slab = slab;
	next_slab_sector = cached_next_sector[slab];
      } else {
	if (compare_records(slab*20 + cached_next_sector[slab], next_slab*20 + next_slab_sector, field_id) <= 0) {
	  next_slab = slab;
	  next_slab_sector = cached_next_sector[slab];
	}
      }
    }
    
    if (next_slab==0xff) break;

#ifdef DEBUG_MERGE
    fprintf(stderr,"DEBUG: Writing sorted record to T%d,S%d from slab=%d,T%d,S%d (stop track=%d)\n",
	    out_track,out_sector,
	    next_slab,cached_track[next_slab], cached_next_sector[next_slab],
	    cached_track_stop[next_slab]
	    );    
#endif

    // Copy the record we need into the sector buffer
    lcopy(WORK_BUFFER_ADDRESS + (next_slab*20L + cached_next_sector[next_slab])*512L,
	  SECTOR_BUFFER_ADDRESS,512);
    
    // Write this record to disk
    if (write_sector(0,out_track,out_sector)) return 5;
    out_sector++;
    if (out_sector==20) {
      out_sector=0;
      out_track++;
    }
    
    // Bump next sector of the slab we've just used
    next_slab_sector++;
    if (next_slab_sector==20) {
      cached_track[next_slab]++;
      if (cached_track[next_slab]<cached_track_stop[next_slab]) {
	// Read the track
#ifdef DEBUG_MERGE
	fprintf(stderr,"DEBUG: Reading track %d for slab %d\n",cached_track[next_slab],next_slab);
#endif
	for(s=0;s<20;s++) {
	  if (read_sector(1,cached_track[next_slab],s)) return 6;
	  lcopy((unsigned long)SECTOR_BUFFER_ADDRESS,WORK_BUFFER_ADDRESS + next_slab*(512L*20) + s*512L, 512);
	}
	next_slab_sector=0;
      }
    }
    cached_next_sector[next_slab]=next_slab_sector;
  
  } while(next_slab!=0xff);

  return 0;
}
