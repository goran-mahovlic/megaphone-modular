#ifndef INDEX_H
#define INDEX_H

extern unsigned char index_mapping_table[256];

char index_update_from_buffer(unsigned char disk_id, unsigned int record_number);
char disk_reindex(unsigned char field);
char index_update_from_buffer(unsigned char disk_id, unsigned int record_number);
char contacts_reindex(unsigned char contacts_disk_id);

#endif
