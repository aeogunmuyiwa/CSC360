#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

// Super block
struct superblock_t {
 uint16_t block_size;
 uint32_t block_count;
 uint32_t fat_start_block;
 uint32_t fat_block_count;
 uint32_t dir_start_block;
 uint32_t dir_block_count;
 uint32_t res_blocks;
 uint32_t alloc_blocks;
 uint32_t free_blocks;
} PACKED;
typedef struct superblock_t superblock_t;
