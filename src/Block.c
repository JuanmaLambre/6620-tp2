#include "config.h"

int LRU_COUNT = 1;

typedef struct {
    int last_used;
    char valid;
    char data[BLOCK_SIZE];
    int tag;
    char bit_D;
} Block;

void Block_init(Block *self) {
    self->last_used = 0;
    self->tag = -1;
    self->bit_D = 0;
    self->valid = 0;
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        self->data[i] = 0;
    }
}

void Block_update_lru(Block *self) {
    self->last_used = LRU_COUNT++;
}

void Block_read(Block *self, Block *src, int tag) {
    Block_init(self);
    self->valid = 1;
    Block_update_lru(self);
    self->tag = tag;
    memcpy(self->data, src->data, BLOCK_SIZE);
}   
