#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

//size of entries in bytes
#define SIZE_DIR_ENTRY 64
#define SIZE_FAT_ENTRY 4
#define FILE_NAME_START 27
#define FILE_NAME_SIZE 31
#define FILE_SIZE 4
#define MODIFY_TIME_START 20
#define MODIFY_TIME_SIZE 7

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

// struct dir_entries_t {
//     char file_type;
//     int file_size;
//     char *file_name;
//     char *modified_time;
// }
// typedef struct dir_entries_t dir_entries_t;

int getSuperBlockInfo(char *mmap, int offset, int length);
char *getAllSuperBlock(char *filename);
int getBlockSize(char *mmap);
void checkAllocation(char *temp);
