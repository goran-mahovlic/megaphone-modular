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
      printf("%d:%d:",
	     buffers.search.record_numbers[i],
	     buffers.search.scores[i]);
      unsigned char record[RECORD_DATA_SIZE];
      if (read_record_by_id(0,buffers.search.record_numbers[i], record))
	printf("<could not read record>\n");
      else {
	unsigned int firstNameLen, lastNameLen, phoneNumberLen, unreadCountLen;
	unsigned char *firstName = find_field(record,RECORD_DATA_SIZE,FIELD_FIRSTNAME,&firstNameLen);
	unsigned char *lastName = find_field(record,RECORD_DATA_SIZE,FIELD_LASTNAME,&lastNameLen);
	unsigned char *phoneNumber = find_field(record,RECORD_DATA_SIZE,FIELD_PHONENUMBER,&phoneNumberLen);
	unsigned char *unreadCount = find_field(record,RECORD_DATA_SIZE,FIELD_UNREAD_MESSAGES,&unreadCountLen);
	int unreadMessageCount=0;
	if (unreadCount) unreadMessageCount=unreadCount[0]+(unreadCount[1]<<8);
	if (firstName||lastName||phoneNumber) {
	  printf("%s:%s:%s:%d\n",
		 firstName?(char *)firstName:"<no first name>",
		 lastName?(char *)lastName:"<no last name>",
		 phoneNumber?(char *)phoneNumber:"<no phone number>",
		 unreadMessageCount);
	}
      }
    }
  }
  
  search_query_release();  
  
  fprintf(stderr,"INFO: Completed.\n");
  return 0;
}
