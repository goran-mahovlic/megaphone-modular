#include "includes.h"
#include "slab.h"

unsigned char t,t_stop,s;
unsigned long rec_addr;

void slab_set_geometry(unsigned char slab_number)
{
  // Read in 80KB = 8 tracks of data at 0x40000
  t=1+(slab_number<<SLAB_TRACK_BITS);
  t_stop=t+SLAB_TRACKS;
  rec_addr = WORK_BUFFER_ADDRESS;

  // Skip directory track
  if (t_stop>=40) t_stop++;
  if (t>=40) t++;
  if (t_stop>81) t_stop=80;

  return;
}

char slab_read(unsigned char disk_id, unsigned char slab_number)
{

  slab_set_geometry(slab_number);
  
  // Load the slab into RAM
  for(;t<t_stop;t++) {
    for(s=0;s<20;s++) {
      if (t==40) t++;
      if (read_sector(disk_id,t,s)) fail(1);
      lcopy((unsigned long)SECTOR_BUFFER_ADDRESS,rec_addr,512);

#ifdef DEBUG_READ_TRACK_1
      if (t==1) {
	fprintf(stderr,"DEBUG: Slab read T%d, S%d from disk_id=%d\n",t,s,disk_id);
	dump_bytes("slab read",rec_addr,512);
      }
#endif      

      rec_addr+=512;
    }
  }

  return 0;
}

char slab_write(unsigned char disk_id, unsigned char slab_number)
{
  // Write out 80KB = 8 tracks of data at 0x40000
  slab_set_geometry(slab_number);

  // Save the slab from RAM
  for(;t<t_stop;t++) {
    for(s=0;s<20;s++) {
      if (t==40) t++;
      lcopy(rec_addr,(unsigned long)SECTOR_BUFFER_ADDRESS,512);
      if (write_sector(disk_id,t,s)) fail(1);
      
      rec_addr+=512;
    }
  }
  
  return 0;
}
