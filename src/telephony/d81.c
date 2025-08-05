// Include file for either MEGA65 or Linux compilation
#include "includes.h"

unsigned char header_template[32]={
  40,3, // First directory sector
  0x44, // DOS Version ('D')
  // Disk label, 0xa0 padded
  0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,
  0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,
  // 2x 0xa0 padding bytes
  0xa0,0xa0,
  // Disk ID
  '6','5',
  // 0xa0 padding
  0xa0,
  // DOS type ('3D')
  '3',0x44,
  // 4x 0xa0 padding
  0xa0,0xa0,0xa0,0xa0,
  // 1x 0x00 padding
  0x00};

unsigned char bam_template[8]={
  40,2, // Sector number of next BAM sector
  0x44, // DOS Version
  0xbb, // 1s complement of version (i.e., 0xff XOR 0x44 = 0xbb)
  '6','5', // Disk ID
  0xc0, // I/O byte: bit 7 = verify on, bit 6 = check sector header CRC
  0x00 // Auto-boot flag
};

// We only care about the 19 bytes of relevance for non-REL non-GEOS files
unsigned char directory_entry_template[20]={
  0xC1, // SEQ< (read-only)
  1,0, // Starts on track 1 (track 0 means unused, i.e., track numbers are 1 -- 80), sector 0
  // Filename (PETSCII, 0xa0 padded)
  0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,
  0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0
};

unsigned char all_sectors_filename[16]={
  'A','L','L',' ','S','E','C','T','O','R','S',
  0xa0,0xa0,0xa0,0xa0,0xa0
};

unsigned char ascii_to_petscii(unsigned char x)
{
  // a–z → 193–218
  if (x >= 'a' && x <= 'z')
    return x - 'a' + 193;
  
  // 0–9 and most common punctuation (ASCII 32–64 and 91–96)
  if ((x >= 32 && x <= 64) || (x >= 91 && x <= 96))
    return x;
  
  // A–Z → same in PETSCII
  if (x >= 'A' && x <= 'Z')
    return x;
  
  // default to question mark for unsupported characters
  return '?';
}

/*
  Format a D81 disk image to contain a single file that
  occupies the entire disk. Allocate 24 directory entries.

  Set record numbers in the start of each data sector, and leave sector 0 blank as the "record BAM".
*/
void format_image_fully_allocated(char drive_id,char *header, char withSectorMarkers)  
{
  unsigned char i,j;
  unsigned int record_number=0;

  /* Header + BAM Sector 1
     ---------------------------------------- */
  
  // Erase sector buffer
  lfill((unsigned long)SECTOR_BUFFER_ADDRESS,0x00,512);
  // Clear header name
  lfill((unsigned long)&header_template[4],0xa0,16);
  // Set header name
  for(i=0;(i<16) && (header[i]);i++) {
    header_template[4+i] = ascii_to_petscii(header[i]);
  }
  lcopy((unsigned long)header_template,(unsigned long)SECTOR_BUFFER_ADDRESS, sizeof(header_template));

  // BAM is easy: Just a valid header, and leave all bitmap bytes $00, to
  // indicate that there is no free space on the disk.
  lcopy((unsigned long)bam_template,(unsigned long)(SECTOR_BUFFER_ADDRESS + 0x100),sizeof(bam_template));
    
  write_sector(drive_id,40,0);

  /* BAM Sector 2 + First directory sector */

  // Erase sector buffer
  lfill((unsigned long)SECTOR_BUFFER_ADDRESS,0x00,512);

  // BAM Sector 2 is the same as BAM Sector 1, except that as it is the last BAM Sector,
  // the link to the next sector is $00,$FF, to indicate end of "file", and that it uses
  // all the bytes.
  lcopy((unsigned long)&bam_template[2],(unsigned long)(SECTOR_BUFFER_ADDRESS + 0x002),sizeof(bam_template));
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+0),0x00);
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+1),0xFF);

  // We assume that we always need 16 directory entries, so setup sector link to
  // track 40 sector 4.
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+0x100),40);
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+0x101),4);

  // Set filename of "all sectors" file
  lcopy((unsigned long)all_sectors_filename,(unsigned long)&directory_entry_template[3],16);

  // Create directory entry
  lcopy((unsigned long)directory_entry_template,(unsigned long)(SECTOR_BUFFER_ADDRESS + 0x102), sizeof(directory_entry_template));
  // Clean up after scribbling over the directory entry template
  lfill((unsigned long)&directory_entry_template[3],0xa0,16);

  // Set number of blocks in file to 3160
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+0x100+0x1e),3160 & 0xff);
  lpoke((unsigned long)(SECTOR_BUFFER_ADDRESS+0x100+0x1f),(3160 >> 8));

  
  write_sector(drive_id,40,1);
  
  /* Directory sectors 2 and 3 */

  // Erase sector buffer
  lfill(SECTOR_BUFFER_ADDRESS,0x00,512);

  // Directory sector links
  lpoke(SECTOR_BUFFER_ADDRESS+0x000,40);
  lpoke(SECTOR_BUFFER_ADDRESS+0x001,5);
  lpoke(SECTOR_BUFFER_ADDRESS+0x100,0);
  lpoke(SECTOR_BUFFER_ADDRESS+0x101,0xff);

  write_sector(drive_id,40,2);

  /* Iterate through the entire disk and write sector links */

  // Tracks
  for(i=1;i<=80;i++) {
    // Don't touch the directory sector
    if (i==40) continue;

    // Physical sectors
    for(j=0;j<20;j++) {
      
      // Next sector links
      // Sector numbers are 0 -- 39, not 1 -- 40

      // First half always links to 2nd half of physical sector
      lpoke(SECTOR_BUFFER_ADDRESS+0x000,i);
      lpoke(SECTOR_BUFFER_ADDRESS+0x001,j*2+1);

      // Write record identifier in every physical sector.
      // Except sector 0. But writing 0 to sector zero is really the same thing...
      if (withSectorMarkers) {
	lpoke(SECTOR_BUFFER_ADDRESS+0x002, record_number & 0xff);
	lpoke(SECTOR_BUFFER_ADDRESS+0x003, record_number>>8);
      }
	// Note that record numbers skip track 40
      record_number++;
      
      // Second half points to next physical sector, or to first
      // sector of next track (or track 41 if we are on track 39,
      // so that we skip the directory).
      if (j<(20-1)) {
	lpoke(SECTOR_BUFFER_ADDRESS+0x100,i);
	lpoke(SECTOR_BUFFER_ADDRESS+0x101,j*2+1+1);
      } else {
	lpoke(SECTOR_BUFFER_ADDRESS+0x100, (i==39)?41:(i+1));
	lpoke(SECTOR_BUFFER_ADDRESS+0x101,0);
      }

      if (i==80&&j==(20-1)) {
	// Terminate chain at end of disk

	lpoke(SECTOR_BUFFER_ADDRESS+0x100, 0x00);
	lpoke(SECTOR_BUFFER_ADDRESS+0x101, 0xff);
      }

      write_sector(drive_id,i,j);
      
    }
  }
  
}
