#include <stdio.h>
#include <stdlib.h>

extern unsigned char sector_buffer[512];
#define SECTOR_BUFFER_ADDRESS ((unsigned long long) &sector_buffer[0])

void hal_init(void);
void lpoke(unsigned long long addr, unsigned char val);
void lfill(unsigned long long addr, unsigned char val, unsigned int len);
void lcopy(unsigned long long src, unsigned long long dest, unsigned int len);
char write_sector(unsigned char drive_id, unsigned char track, unsigned char sector);
char read_sector(unsigned char drive_id, unsigned char track, unsigned char sector);
char mount_d81(char *filename, unsigned char drive_id);
char create_d81(char *filename);
char mega65_mkdir(char *dir);
char mega65_cdroot(void);
char mega65_chdir(char *dir);

void format_image_fully_allocated(char drive_id,char *header);

char sort_d81(char *name_in, char *name_out, unsigned char field_id);

void dump_sector_buffer(char *m);
void dump_bytes(char *msg, unsigned char *d, int len);



