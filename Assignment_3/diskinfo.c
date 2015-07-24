/* Title: diskinfo
 * Author: Brett A. Blashko
 * ID: V00759982
 * Purpose:
 *
*/

#include "diskinfo.h"

struct superblock_t sb_t;
/*--readFAT(Char *mmap, int offset, int length);
 *
*/
void readFAT(char *mmap)
{

    int offset = sb_t.fat_start_block;
    int length = sb_t.fat_block_count;
    int block_size = sb_t.block_size;

    int FAT_start = offset * block_size;
    int FAT_end = FAT_start + length * block_size;
    int size = FAT_end - FAT_start;

    char *temp = (char *)malloc(size);
    memcpy(temp, mmap+FAT_start, size);

    int i = 0;
    int free_blocks = 0;
    int reserved_blocks = 0;
    int alloc_blocks = 0;

    for(i = 0; i < size; i+=4)
    {
        int value = 0;
        int num_bytes = 3;
        for(int j = i; j < i + 4; j++)
        {
            value += (int)temp[j]<<(8*(num_bytes));
            num_bytes--;
        }
        if(value == 0)
            free_blocks++;
        else if(value == 1)
            reserved_blocks++;
        else
            alloc_blocks++;
    }
    sb_t.free_blocks = free_blocks;
    sb_t.res_blocks = reserved_blocks;
    sb_t.alloc_blocks = alloc_blocks;
}

/*--getSuperBlockInfo();
 *Purpose:
*/
int getSuperBlockInfo(char *mmap, int offset, int length)
{
    char *temp = (char *)malloc(sizeof(char) *length);
    memcpy(temp, mmap+offset, length);

    int i = 0;
    int retVal = 0;

    for(i = 0; i < length; i++)
    {
        retVal += ((int)temp[i]<<(8*(length - i - 1)));
    }
    free(temp);
    return retVal;
}

/*--getBlockSize(char *mmap, int offset, int length)
 *
*/
int getBlockSize(char *mmap)
{
    int *temp = malloc(sizeof(int));
    int *temp2 = malloc(sizeof(int));
    int retVal;
    *temp = mmap[8];
    *temp2 = mmap[9];
    retVal = ((*temp)<<8)+(*temp2);
    free(temp);
    free(temp2);
    return retVal;
}

/* Purpose: Main flow of the program
 *
*/
int main(int argc, char **argv)
{
    char *filename = argv[1];
    struct stat stats;
    char *p;

    // struct superblock_t sb_t;

    int open_file = open(filename, O_RDONLY);
    if(open_file == -1)
    {
        printf("[ERROR]: %s was not found\n", filename);
        return -1;
    }
    fstat(open_file, &stats);
    p = mmap(NULL, stats.st_size, PROT_READ, MAP_SHARED, open_file, 0);

    sb_t.block_size = getBlockSize(p);
    sb_t.block_count = getSuperBlockInfo(p, 10, 4);
    sb_t.fat_start_block = getSuperBlockInfo(p, 14, 4);
    sb_t.fat_block_count = getSuperBlockInfo(p, 18, 4);
    sb_t.dir_start_block = getSuperBlockInfo(p, 22, 4);
    sb_t.dir_block_count = getSuperBlockInfo(p, 26, 4);

    readFAT(p);

    printf("\nSuper Block Information:\n");
    printf("Block Size: %d\n", sb_t.block_size);
    printf("Block Count: %d\n", sb_t.block_count);
    printf("FAT Starts: %d\n", sb_t.fat_start_block);
    printf("FAT Blocks: %d\n", sb_t.fat_block_count);
    printf("Root Directory Start: %d\n", sb_t.dir_start_block );
    printf("Root Directory Blocks: %d\n", sb_t.dir_block_count);
    printf("\nFAT Information:\n");
    printf("Free Blocks: %d\n", sb_t.free_blocks);
    printf("Reserved Blocks: %d\n", sb_t.res_blocks);
    printf("Allocated Blocks: %d\n", sb_t.alloc_blocks);

    return 0;

}
