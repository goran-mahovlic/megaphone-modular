#include "includes.h"
#include "buffers.h"
#include "search.h"

void usage(void)
{
  fprintf(stderr,"usage: search <disk> <index> <query>\n");
  exit(-1);
}

int main(int argc,char **argv)
{
  if (argc!=4) usage();

  hal_init();

  // Mount disk and index
  if (mount_d81(argv[1],0)) return 1;
  if (mount_d81(argv[2],1)) return 2;
  
  if (search_query_init()) return 3;

  for(int i=0;argv[3][i];i++) search_query_append(argv[3][i]);

  // Include only matches in the results, rather than just preparing to sort them
  // by matchiness.
  search_collate(SEARCH_FILTERED);
  // Then sort them by descending score
  search_sort_results_by_score();
  
  for(int i=0;i<USABLE_SECTORS_PER_DISK;i++) {
    if (buffers.search.scores[i]) {
      printf("%d:%d:\n",
	     buffers.search.record_numbers[i],
	     buffers.search.scores[i]);
    }
  }
  
  search_query_release();  
  
  fprintf(stderr,"INFO: Completed.\n");
  return 0;
}
