#include "includes.h"

char slab_read(unsigned char slab_number)
{
  // Read in 80KB = 8 tracks of data at 0x40000
  unsigned char t=1+(slab_number<<3);
  unsigned char t_stop=t+8;
  unsigned char s;
  unsigned long rec_addr = WORK_BUFFER_ADDRESS;
  
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

  return 0;
}
