#include <config.h>
#include <Block.c>
#include <Memory.c>

#define NO_BLOCKS CACHE_SIZE/NO_WAYS/BLOCK_SIZE


typedef struct {
    Block blocks[NO_WAYS][NO_BLOCKS];
    int access_count;
    int misses_count;
    Memory* memory;
} Cache;


void Cache_init(Cache* self, Memory* memory) {
    self->memory = memory;
    self->access_count = 0;
    self->misses_count = 0;
    for (int w = 0; w < NO_WAYS; ++w) {
        for (int b = 0; b < NO_BLOCKS; ++b) {
            Block_init(&self->blocks[w][b]);
        }
    }
}

void Cache_read_block(int blocknum) {
    
}
