#include "config.h"

typedef struct {
    int last_write;
    char valid;
    char data[BLOCK_SIZE];
    char tag;
    char bit_D;
} Block;


void Block_init(Block* self) {
    self->last_write = 0;
    self->tag = 0;
    self->bit_D = 0;
    self->valid = 0;
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        self->data[i] = 0;
    }
}

