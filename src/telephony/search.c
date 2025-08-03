#include "includes.h"
#include "buffers.h"

/*
  Reset search of a records D81.
  The search result structure will contain all allocated records
  in positive position order.

  Requires that the disk being searched is loaded as disk 0, and the
  index is loaded as disk 1.
*/
char search_query_init(void)
{
  // Shared data structures locked by another sub-system
  if (buffers.lock&&buffers.lock != LOCK_SEARCH) return 99;

  buffers.lock = LOCK_SEARCH;
  
  // Reset all scores to zero
  lfill((unsigned long)&buffers.search.scores,0x00, USABLE_SECTORS_PER_DISK);

  // Reset query (used so that we can delete diphthongs from the query.
  buffers.search.query_length = 0;

  // Set score of all valid records to 1, so that we start with an unfiltered
  // view, but that ignores unallocated records.

  // Read record BAM of the disk being searched
  if (read_sector(0,1,0)) return 2;  
  lcopy(SECTOR_BUFFER_ADDRESS,(unsigned long)buffers.search.sector_buffer,512);
  
  // Transcribe bits into scores  
  buffers.search.byte=0;
  buffers.search.bit=1;
  for(buffers.search.r=1;buffers.search.r<USABLE_SECTORS_PER_DISK;
      buffers.search.r++) {
    if (buffers.search.sector_buffer[buffers.search.byte]
	&(1<<buffers.search.bit))
      buffers.search.scores[buffers.search.r]=1;
    buffers.search.bit++;
    if (buffers.search.bit==8) {
      buffers.search.byte++;
      buffers.search.bit=0;
    }
  }
  
  return 0;
}

char search_query_release(void)
{
  // Shared data structures locked by another sub-system
  if (buffers.lock&&buffers.lock != LOCK_SEARCH) return 99;

  buffers.lock = LOCK_FREE;

  return 0;
}

char search_query_append(unsigned char c)
{
  // Shared data structures locked by another sub-system,
  // or 
  if (buffers.lock != LOCK_SEARCH) return 99;

  // Query too long?
  if (buffers.search.query_length >= SEARCH_MAX_QUERY_LENGTH) return 1;

  // Append character
  buffers.search.query[buffers.search.query_length++]=c;

  // Nothing to do yet if only one char, as our index is based on diphthongs
  if (buffers.search.query_length==1) return 0;



  // XXX finish implementing
  return 1;
}
