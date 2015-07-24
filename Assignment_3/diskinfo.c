/* Title: diskinfo
 * Author: Brett A. Blashko
 * ID: V00759982
 * Purpose:
 *
*/

#include "diskinfo.h"

struct superblock_t sb_t;

void checkAllocation(char *temp)
{
    if(temp == NULL)
    {
        printf("[ERROR]: Failed to allocate memory in function readFATInfo()");
        exit(-1);
    }
}

/*--readFAT(Char *mmap, int offset, int length);
 * Purpose: Read bytes in the File Allocation Table!
 *          get # of free blocks, allocated blocks, and reservered blocks.
 * Input: mmap
 * output: N/A
*/
void readFATInfo(char *mmap)
{

    //set size parameters
    int offset = sb_t.fat_start_block;
    int length = sb_t.fat_block_count;
    int block_size = sb_t.block_size;

    //cacluate locations of length of the start and end of fat
    int FAT_start = offset * block_size;
    int FAT_end = FAT_start + length * block_size;
    int size = FAT_end - FAT_start;

    //allocate memmpory
    char *temp = (char *)malloc(size);
    checkAllocation(temp);

    //copy important bytes of memory into temp
    memcpy(temp, mmap+FAT_start, size);

    int i = 0;
    int free_blocks = 0;
    int reserved_blocks = 0;
    int alloc_blocks = 0;

    //read the last byte of every FAT entry
    //increment different counts depending on byte value.
    for(i = 3; i < size; i+=4)
    {
        int value = (int)temp[i];
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

    free(temp);
}

/*--getSuperBlockInfo();
 * Purpose: Get information found in the mmap inbetween the offset and the offset + length
 * Input: mmap, memory offset, length of bytes to read.
 * Output: determined value found in the mmap
*/
int getSuperBlockInfo(char *mmap, int offset, int length)
{
    //allocate memory in temp.
    char *temp = (char *)malloc(sizeof(char) *length);
    checkAllocation(temp);
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
 * Purpose: get the size of the blocks
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

/* --Main()
 * Purpose: Main flow of the program
*/
int main(int argc, char **argv)
{
    char *filename = argv[1];
    struct stat stats;
    char *p;

    int open_file = open(filename, O_RDONLY);
    if(open_file == -1)
    {
        printf("[ERROR]: %s was not found\n", filename);
        return -1;
    }

    //make a mmap from open_file
    fstat(open_file, &stats);
    p = mmap(NULL, stats.st_size, PROT_READ, MAP_SHARED, open_file, 0);

    //get all information needed to locate additional information in the File Allocation Table
    sb_t.block_size = getBlockSize(p);
    sb_t.block_count = getSuperBlockInfo(p, 10, 4);
    sb_t.fat_start_block = getSuperBlockInfo(p, 14, 4);
    sb_t.fat_block_count = getSuperBlockInfo(p, 18, 4);
    sb_t.dir_start_block = getSuperBlockInfo(p, 22, 4);
    sb_t.dir_block_count = getSuperBlockInfo(p, 26, 4);

    //get allocated blocks, reservered blocks, free blocks
    readFATInfo(p);

    //print info
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
    printf("Allocated Blocks: %d\n\n", sb_t.alloc_blocks);

    return 0;

}
