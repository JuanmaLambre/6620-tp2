#include "config.h"
#include "Block.c"
#include "Memory.c"

#include "math.h"

#define NO_BLOCKS (CACHE_SIZE/NO_WAYS/BLOCK_SIZE)
#define INDEX_SIZE (log(NO_BLOCKS)/log(2))
#define OFFSET_SIZE (log(BLOCK_SIZE)/log(2))
#define TAG_SIZE (ADDR_SIZE-INDEX_SIZE-OFFSET_SIZE)


typedef unsigned char uchar;

typedef struct {
    Memory memory;
    Block blocks[NO_WAYS][NO_BLOCKS];
    int access_count;
    int misses_count;
} Cache;

Cache self;


void init() {
    self.access_count = 0;
    self.misses_count = 0;
    for (int w = 0; w < NO_WAYS; ++w) {
        for (int b = 0; b < NO_BLOCKS; ++b) {
            Block_init(&self.blocks[w][b]);
        }
    }
    Memory_init(&self.memory);
}

int get_index(int addr) {
    return (int)(addr / pow(2, OFFSET_SIZE)) % (int)(pow(2, INDEX_SIZE));
}

int get_offset(int addr) {
    return addr % BLOCK_SIZE;
}

int get_tag(int addr) {
    return (int)(addr / pow(2, INDEX_SIZE + OFFSET_SIZE));
}

void read_block(int blocknum) {
    // Doesnt check if block is dirty
    int tag = get_tag(blocknum * BLOCK_SIZE);
    int set = get_index(blocknum * BLOCK_SIZE);
    int way = find_lru(set);
    Block_read(&self.blocks[way][set], &self.memory.blocks[blocknum], tag);
}

Block* cache_address(int addr) {
    int set = find_set(addr);
    int way = find_lru(set);
    if (is_valid(way, set) && is_dirty(way, set)) {
        write_block(way, set);
    }

    read_block(get_blocknum(addr));
    return &self.blocks[way][set];
}

int read_byte(int address) {
    if (address >= MEMORY_SIZE) {
        fprintf(stderr, "Address %d overflows\n", address);
        return -1;
    }
    int tag = get_tag(address);
    int offset = get_offset(address);
    ++self.access_count;
    int set = find_set(address);
    for (int way = 0; way < NO_WAYS; ++way) {
        Block block = self.blocks[way][set];
        if (block.valid && block.tag == tag) {
            Block_update_lru(&block);
            return block.data[offset];
        } 
    }
    // Miss in cache
    ++self.misses_count;
    Block *b = cache_address(address); // This updates lru
    return b->data[offset];
}

int find_lru(int setnum) {
    int minway = 0;
    for (int w = 0; w < NO_WAYS; ++w) {
        Block b = self.blocks[w][setnum];
        if (!b.valid)
            return w;
        else if (b.last_used < self.blocks[minway][setnum].last_used)
            minway = w;
    }
    return minway;
}

int is_valid(int way, int set) {
    return self.blocks[way][set].valid;
}

int is_dirty(int way, int set) {
    return self.blocks[way][set].bit_D;
}

int get_blocknum(int address) {
    return (int)(address/BLOCK_SIZE);
}

void write_block(int way, int set) {
    Block block = self.blocks[way][set];
    int block_num = (int)(block.tag * pow(2, INDEX_SIZE) + set);
    Memory_write_block(&self.memory, &block, block_num);
}

int write_byte(int address, char value) {
    // Write-Allocate politic => update cache to write
    if (address >= MEMORY_SIZE) {
        fprintf(stderr, "Address %d overflows\n", address);
        return -1;
    }
    Block* block = NULL;
    int tag = get_tag(address);
    int offset = get_offset(address);
    int set = find_set(address);
    int way = 0;
    ++self.access_count;

    for (; way < NO_WAYS && !block; ++way) {
        Block *b = &self.blocks[way][set];
        if (b->valid && b->tag == tag)
            block = b;
    }

    if (!block) {
        ++self.misses_count;
        block = cache_address(address);
    }

    block->data[offset] = value;
    block->bit_D = 1;
    Block_update_lru(&block);
    return (unsigned char) value;
}

int find_set(int addr) {
    return get_index(addr);
}

int get_miss_rate() {
    return (int)(100.0*self.misses_count/self.access_count);
}
