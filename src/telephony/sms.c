#include "includes.h"
#include "contacts.h"
#include "records.h"
#include "search.h"

char sms_rx(unsigned char *phone, unsigned int timestampAztecTime,
	    unsigned char *message)
{      
  // 1. Work out which contact the message is to/from
  unsigned int contact_ID = search_contact_by_phonenumber(phoneNumber);
  
  // 2. Retreive that contact (or if no such contact, then use the "UNKNOWN NUMBERS" pseudo-contact?)
  // contact_find_by_phonenumber() will return contact 1 always
  fprintf(stderr,"DEBUG: Phone number '%s' is contact %d\n",phoneNumber,contact_ID);

  if (buffers_lock(LOCK_TELEPHONY)) return 99;
  
  if (read_record_by_id(0, contact_ID,buffers.telephony.contact)) {
    buffers_unlock(LOCK_TELEPHONY);
    return 1;
  }
			
  // 3. Increase unread message count by 1, and write back.
  unsigned int unreadMessageCountLen = 0;
  unsigned char *unreadMessageCount
    = find_field(buffers.telephony.contact,RECORD_DATA_SIZE,FIELD_UNREAD_MESSAGES,
		 &unreadMessageCountLen);
  if (unreadMessageCountLen == 2) {
    unreadMessageCount[0]++;
    if (!unreadMessageCount[0]) {
      unreadMessageCount[1]++;
      if (!unreadMessageCount[1]) {
	// Saturate rather than wrap at 64K unread messages
	unreadMessageCount[0]=x0ff;
	unreadMessageCount[1]=x0ff;
      }
    } 
    write_record_by_id(0,contact_ID,buffers.telephony.contact);
  } else {
    // No unread message count in the contact. Silently ignore for now.
  }  
  
  buffers_unlock(LOCK_TELEPHONY);
  
  // 4. Obtain contact physical ID from contact record, and then find and open the message D81 for that conversation.
  // Actually, the way we do the indexes on the unsorted contact D81 means that contact_ID
  // should already be the physical location of the contact in that D81.
  // So we just need to do the path magic and them mount it, and the message body index for it.

  
  // 5. Allocate message record in conversation
  
  // 6. Build message and store.

  // 7. Update used message count in conversation (2nd half of BAM sector?)

  // 8. Update thread index for this message
 
}
