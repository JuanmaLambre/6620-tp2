#ifndef MEMORY_C
#define MEMORY_C

#include "config.h"

#define BLOCKS_COUNT MEMORY_SIZE/BLOCK_SIZE

typedef struct {
    Block blocks[BLOCKS_COUNT];
} Memory;

void Memory_init(Memory *self) {
    for (int b = 0; b < BLOCKS_COUNT; ++b) {
        Block_init(&self->blocks[b]);
    }
}

void Memory_write_block(Memory *self, Block *src, int blockNo) {
    Block *memblock = self->blocks + blockNo;
    memcpy(memblock->data, src->data, BLOCK_SIZE);
}

#endif
