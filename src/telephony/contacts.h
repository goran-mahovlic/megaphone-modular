#ifndef CONTACTS_H
#define CONTACTS_H

char build_contact(unsigned char buffer[508],unsigned int *bytes_used,
		   unsigned char *firstName,
		   unsigned char *lastName,
		   unsigned char *phoneNumber,
		   unsigned int unreadCount);
char write_contact_by_id(unsigned char drive_id,unsigned int id,unsigned char *buffer);
char read_contact_by_id(unsigned char drive_id,unsigned int id,unsigned char *buffer);

#endif
