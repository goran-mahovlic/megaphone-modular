#include "includes.h"
#include <string.h>

#include "buffers.h"
#include "search.h"

void usage(void)
{
  fprintf(stderr,"usage: thread <contact name or number> [ranked] [message thread filter]\n");
  exit(-1);
}

int main(int argc,char **argv)
{
  if (argc<2) usage();

  hal_init();

  char *query=NULL;
  int ranked=0;
  
  if (argc==4) {
    if (!strcasecmp("ranked",argv[2])) ranked=1;
    else {
      usage();
    }
    query=argv[3];
  }
  if (argc==3) {
    if (!strcasecmp("ranked",argv[2])) ranked=1;
    else query=argv[2];      
  }
  
  mega65_chdir("PHONE");
    
  // Mount disk and index
  if (mount_d81("CONTACT0.D81",0)) return 1;
  if (mount_d81("IDXALL-0.D81",1)) return 1;

  unsigned int score_threshold=1;

  // Dodgy match selecitivity filter  
  if (argv[1]&&(strlen(argv[1]?argv[1]:"")>3))
    score_threshold=strlen(argv[1])*0.75;
  fprintf(stderr,"DEBUG: Score threshold is %d, argv[1]=%s\n",
	  score_threshold,argv[1]?argv[1]:"<empty>");

  // Get list of candidate contacts
  if (search_query_init()) return 3;
  if (argc>1&&argv[1]) search_query_append_string((unsigned char *)argv[1]);
  search_collate(score_threshold); // apply match threshold filter
  search_sort_results_by_score();

  if (!buffers.search.result_count) {
    fprintf(stderr,"ERROR: No matching contacts found.\n");
    exit(-1);
  }
  
  for(int i=0;i<buffers.search.result_count;i++) {
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
      } else printf("<no contact fields in record>\n");
    }
  }
  
  search_query_release();  
  
  fprintf(stderr,"INFO: Completed.\n");
  return 0;
}
