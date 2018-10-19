#ifndef MEMORY_C
#define MEMORY_C

#include <config.h>

#define BLOCKS_COUNT MEMORY_SIZE/BLOCK_SIZE


typedef struct {
    Block blocks[BLOCKS_COUNT];
} Memory;


#endif
