#include "includes.h"

#include "records.h"

#include <string.h>

char build_contact(unsigned char buffer[508],unsigned int *bytes_used,
		   unsigned char *firstName,
		   unsigned char *lastName,
		   unsigned char *phoneNumber)
{
  // Reserve first two bytes for record number
  *bytes_used=2;

  // Clear buffer (will intrinsically add an end of record marker = 0x00 byte)
  lfill((unsigned long)&buffer[0],0x00,508);
  
  // +1 so strings are null-terminated for convenience.
  if (append_field(buffer,bytes_used,508, FIELD_FIRSTNAME, firstName, strlen((char *)firstName)+1)) return 1;
  if (append_field(buffer,bytes_used,508, FIELD_LASTNAME, lastName, strlen((char *)lastName)+1)) return 2;
  if (append_field(buffer,bytes_used,508, FIELD_PHONENUMBER, phoneNumber, strlen((char *)phoneNumber)+1)) return 3;

  return 0;
}

char write_contact_by_id(unsigned char drive_id,unsigned int id,unsigned char *buffer)
{
  // The ID let's us determine the track and sector.

  unsigned char track, sector;

  // Work out where the sector gets written to
  track=1 + id/20;  // Track numbers start at 1
  sector=id - ((track-1)*20);
  if (track>39) track++;

  // And ID of 0 is invalid (it's the record BAM sector).
  if (!id) return 1;

  // fprintf(stderr,"DEBUG: Trying to write contact #%d\n",id);

  // Read original sector in to get inter-sector links
  if (read_sector(drive_id,track,sector)) return 4;

  // Then write the contact body over the data area
  sectorise_record(buffer,(unsigned char *)SECTOR_BUFFER_ADDRESS);

  // dump_bytes("Sectorised Contact",(unsigned char *)SECTOR_BUFFER_ADDRESS,512);

  // And commit it back to disk
  // fprintf(stderr,"DEBUG: write_sector(%d,%d,%d)\n",drive_id,track,sector);
  if (write_sector(drive_id,track,sector)) return 3;

  return 0;
}

char read_contact_by_id(unsigned char drive_id,unsigned int id,unsigned char *buffer)
{
  // The ID let's us determine the track and sector.

  unsigned char track, sector;

  // Work out where the sector gets written to
  track=id/20;
  sector=id - (track*20);
  if (track>39) track++;
  
  // And ID of 0 is invalid (it's the record BAM sector).
  if (!id) return 1;

  if (read_sector(drive_id,track,sector)) return 3;

  desectorise_record((unsigned char *)SECTOR_BUFFER_ADDRESS,buffer);
  
  return 0;
}
