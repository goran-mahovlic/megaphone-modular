#include "includes.h"

// We sort 160 records = 80KB = 1/10th of a disk image at a time.
#define QSORT_MAX_RECORDS 160
#define QSORT_STACK_SIZE  160  // safe headroom

typedef struct {
    unsigned char low;
    unsigned char high;
} partition_t;

struct sort_buffers {
  // Quick sort list of index numbers of the records being sorted
  unsigned char indices[QSORT_MAX_RECORDS];  // indices into your slab records
  // Quick sort manual stack
  partition_t stack[QSORT_STACK_SIZE];
  int stack_ptr = 0;
  
  // A and B record buffers for comparison of records during quick sort
  unsigned char rec_a[RECORD_DATA_SIZE];
  unsigned char rec_b[RECORD_DATA_SIZE];  
};

struct shared_buffers {
  union {
    struct sort_buffers sort;
    struct index_buffers index;
  };
};
