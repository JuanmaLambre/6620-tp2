#include "config.h"
#include "Block.c"
#include "Memory.c"

#define NO_BLOCKS CACHE_SIZE/NO_WAYS/BLOCK_SIZE

// 4 bits index
// 6 bits offset
// 6 bits tag

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

char* getBinary(int number) {
    int c, d, count;
    char *pointer;
    count = 0;
    pointer = (char*)malloc(16+1);

    if (pointer == NULL)
        exit(EXIT_FAILURE);
    for (c = 15 ; c >= 0 ; c--) {
        d = number >> c;
        if (d & 1)
            *(pointer+count) = 1 + '0';
        else
            *(pointer+count) = 0 + '0';
        count++;
    }
    *(pointer+count) = '\0';
    return  pointer;
}

char* getIndex(char* binAddress) {
    int i;
    char* index = (char*)malloc(4+1);
    for (i = 0; i < 4; i++) {
        index[i] = binAddress[6+i];
    }
    index[4] = '\0';
    return index;
}

char* getOffset(char* binAddress) {
    int i;
    char* offset = (char*)malloc(6+1);
    for (i = 0; i < 6; i++) {
        offset[i] = binAddress[10+i];
    }
    offset[6] = '\0';
    return offset;
}

char* getTag(char* binAddress) {
    int i;
    char* tag = (char*)malloc(6+1);
    for (i = 0; i < 6; i++) {
        tag[i] = binAddress[i];
    }
    tag[6] = '\0';
    return tag;
}

int getIntFromBin(char* binary) {
    int result=0, count, len;
    for(count=0,len=strlen(binary); count<len; count++) {
        result<<=1;
        if(binary[count] == '1') {
            result += 1;
        }
    }
    return result;
}


char Cache_read_block(Cache* self, int blocknum) {
        char* binary = getBinary(blocknum);
        char* index = getIndex(binary);
        int indexDec = getIntFromBin(index);
        char* tag = getTag(binary);
        int tagDec = getIntFromBin(tag);
        char* offset = getOffset(binary);
        int offsetDec = getIntFromBin(offset);
        printf("Read %d Tag: %d Index: %d Offset: %d\n", blocknum, tagDec, indexDec, offsetDec);
        
        int found = 2359, i = 0;
        Block block;
        
        while (found == 2359 && i < 4) {
            block = self->blocks[i][indexDec];
            if (block.tag == tagDec) {
                found = 1;
            }
            i++;
        }

        if (found == 1) {
            printf("Data found in cache: %d", block.data[offsetDec]);
            return block.data[offsetDec];
        } 

        // if not found look in memory and update cache
        free(tag);
        free(offset);
        free(index);
        free(binary);
        return "0";
}
