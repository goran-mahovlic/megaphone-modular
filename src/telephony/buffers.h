#ifndef BUFFERS_H
#define BUFFERS_H

#include "records.h"

// We sort 160 records = 80KB = 1/10th of a disk image at a time.
// (This value MUST be < 0xff, as 0xff is used as record number to
//  invalidate the rec_b cache via setting b_idx = 0xff; )
#define QSORT_MAX_RECORDS 160
#define QSORT_STACK_SIZE  160  

typedef struct {
    unsigned char low;
    unsigned char high;
} partition_t;

struct sort_buffers {
  // Quick sort list of index numbers of the records being sorted
  unsigned char indices[QSORT_MAX_RECORDS];  // indices into your slab records
  // Quick sort manual stack
  partition_t stack[QSORT_STACK_SIZE];
  int stack_ptr;
  
  // A and B record buffers for comparison of records during quick sort
  unsigned char rec_a[RECORD_DATA_SIZE];
  unsigned char rec_b[RECORD_DATA_SIZE];
  unsigned char idx_b;
};

struct index_buffers {
  unsigned char rec[RECORD_DATA_SIZE];
};

struct shared_buffers {

#define LOCK_FREE 0x00
#define LOCK_INDEX 0x01
#define LOCK_SORT 0x02
  unsigned char lock;
  
  union {
    struct sort_buffers sort;
    struct index_buffers index;
  };
};

extern struct shared_buffers buffers;

unsigned char buffers_lock(unsigned char module);
unsigned char buffers_unlock(unsigned char module);

#endif
