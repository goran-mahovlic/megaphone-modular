#include "includes.h"

#if WORK_BUFFER_SIZE >= (80*1024)
#define SLAB_TRACKS 8
#define SLAB_TRACK_BITS 3
#define SLAB_COUNT 10
#elif WORK_BUFFER_SIZE >= (40*1024)
#define SLAB_TRACKS 4
#define SLAB_TRACK_BITS 2
#define SLAB_COUNT 20
#elif WORK_BUFFER_SIZE >= (20*1024)
#define SLAB_TRACKS 2
#define SLAB_TRACK_BITS 1
#define SLAB_COUNT 40
#elif WORK_BUFFER_SIZE >= (10*1024)
#define SLAB_TRACKS 1
#define SLAB_TRACK_BITS 0
#define SLAB_COUNT 80
#else
#error WORK_BUFFER_SIZE must be >= 10KB
#endif

char slab_read(unsigned char slab_number);
