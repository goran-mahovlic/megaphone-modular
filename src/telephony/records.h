#define USABLE_SECTORS_PER_DISK (1680-1)

// Fields used for contacts
#define FIELD_FIRSTNAME 0x02
#define FIELD_LASTNAME 0x04
#define FIELD_PHONENUMBER 0x06
#define FIELD_QSODISKIMAGE 0x08
#define FIELD_UNREAD_MESSAGES 0x0a

// Fields used for messages
#define FIELD_MESSAGE_DIRECTION 0x0c
#define FIELD_TIMESTAMP 0x0e
#define FIELD_BODYTEXT 0x10

unsigned int record_allocate_next(unsigned char *bam_sector);
char record_free(unsigned char *bam_sector,unsigned int record_num);

void sectorise_record(unsigned char *record,
		      unsigned char *sector_buffer);
void desectorise_record(unsigned char *sector_buffer,
			unsigned char *record);

char append_field(unsigned char *record, unsigned int *bytes_used, unsigned int length,
		  unsigned char type, unsigned char *value, unsigned int value_length);
char delete_field(char *record, unsigned int *bytes_used, unsigned char type);
char *find_field(char *record, unsigned int bytes_used, unsigned char type, unsigned int *len);






