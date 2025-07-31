// Include file for either MEGA65 or Linux compilation
#include "includes.h"

#include "records.h"

unsigned int record_allocate_next(unsigned char *bam_sector)
{
  /* Find an unallocated sector on the D81 using our native allocation method,
     not the D81's DOS BAM which we always have set to show as completely full.

     We use a simple bitmap for allocation, where 1 = ALLOCATED (not free, like the D81 DOS).
     We can fit 512 x 8 = 4K entries in a single 512 byte sector.
     In fact, we can fit the entire allocation information in the first 254-byte data area
     of the physical sector, since 254x8 = 2,032, but there are only 1,680 sectors.
  */
  unsigned char i,j,b;

  for(i=0;i<(USABLE_SECTORS_PER_DISK/8);i++) {
    // Found a BAM byte that's not full?
    if (bam_sector[2+i]!=0xff) {
      // Work out which bit is clear, set it, and return the allocated record.
      b = bam_sector[2+i];
      for(j=0;j<8;j++) {
	if (!(b&1)) {
	  bam_sector[2+i]|=(1<<j);
	  // Make sure we're not trying to allocate sector 0 (where the BAM lives)
	  if (i+j) return (i<<3)+j;
	}
	b=b>>1;
      }
    }
  }

  // BAM sector is 0, which if we return that, then we mean no sector could be allocated.
  return 0;
}

char record_free(unsigned char *bam_sector,unsigned int record_num)
{
  unsigned char b, bit;
  if (record_num>=USABLE_SECTORS_PER_DISK) return 1;
  b=bam_sector[record_num>>3];
  bit = 1<<(record_num&7);
  if (!(b&bit)) return 2; // Record wasn't allocated
  b&=(0xff-bit);
  bam_sector[record_num>>3]=b;
  return 0;
}

void sectorise_record(unsigned char *record,
		      unsigned char *sector_buffer)
{
  lcopy((unsigned long)&record[0],(unsigned long)&sector_buffer[2],254);
  lcopy((unsigned long)&record[254],(unsigned long)&sector_buffer[256+2],254);
}

void desectorise_record(unsigned char *sector_buffer,
			unsigned char *record)
{
  lcopy((unsigned long)&sector_buffer[2],(unsigned long)&record[0],254);
  lcopy((unsigned long)&sector_buffer[256+2],(unsigned long)&record[254],254);
}

char append_field(unsigned char *record, unsigned int *bytes_used, unsigned int length,
		  unsigned char type, unsigned char *value, unsigned int value_length)
{
  // Insufficient space
  if (((*bytes_used)+1+1+value_length)>=length) return 1;
  // Field too long
  if (value_length > 511) return 2;
  // Type must be even
  if (type&1) return 3;

  record[(*bytes_used)++]=type|(value_length>>8);
  record[(*bytes_used)++]=value_length&0xff;
  lcopy((unsigned long)value,(unsigned long)&record[*bytes_used],value_length);
  (*bytes_used)+=value_length;

  return 0;
}

char delete_field(char *record, unsigned int *bytes_used, unsigned char type)
{
  unsigned int ofs=2;      // Skip record number indicator
  unsigned char deleted=0;

  while(ofs<=(*bytes_used)&&record[ofs]) {
    unsigned int shuffle = 1 + 1 + ((record[ofs]&1)?256:0) + record[ofs+1];

    if ((record[ofs]&0xfe) == type) {
      // Found matching field
      // Now to shuffle it down.
      // On MEGA65, lcopy() has "special" behaviour with overlapping copies.
      // Basically the first few bytes will be read before the first byte is written.
      // The nature of the copy that we are doing, shuffling down, is safe in this context.
      lcopy((unsigned long)&record[ofs+shuffle],(unsigned long)&record[ofs],(*bytes_used)-ofs-shuffle);
      (*bytes_used) -= shuffle;

      deleted++;
      
      // There _shouldn't_ be multiple fields with the same type in a record, but who knows?
      // So we'll just continue and look for any others.
    } else ofs+=shuffle;
  }
  return deleted;
}

char *find_field(char *record, unsigned int bytes_used, unsigned char type, unsigned int *len)
{
  unsigned int ofs=2;  // Skip record number indicator

  while((ofs<=bytes_used)&&record[ofs]) {
    unsigned int shuffle = 1 + 1 + ((record[ofs]&1)?256:0) + record[ofs+1];

    if ((record[ofs]&0xfe) == type) {
      *len = ((record[ofs]&1)?256:0) + record[ofs+1];
      return &record[ofs+2];
    }

    ofs+=shuffle;    
  }
  return NULL;
}
