#define USABLE_SECTORS_PER_DISK (1680-1)

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






