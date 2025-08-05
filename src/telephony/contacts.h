#ifndef CONTACTS_H
#define CONTACTS_H

char build_contact(unsigned char buffer[508],unsigned int *bytes_used,
		   unsigned char *firstName,
		   unsigned char *lastName,
		   unsigned char *phoneNumber,
		   unsigned int unreadCount);
char mount_contact_qso(unsigned int contact);

char to_hex(unsigned char v);

#endif
