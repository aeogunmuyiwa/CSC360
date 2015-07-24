/* Title: diskinfo
 * Author: Brett A. Blashko
 * ID: V00759982
 * Purpose:
 *
*/

#include "commonFunctions.h"

struct superblock_t sb_t;

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
    int entry_size = SIZE_FAT_ENTRY;

    //read the last byte of every FAT entry
    //increment different counts depending on byte value.
    for(i = entry_size-1; i < size; i+= entry_size)
    {
        int value = (int)temp[i];
        if(value == 0)
            sb_t.free_blocks++;
        else if(value == 1)
            sb_t.res_blocks++;
        else
            sb_t.alloc_blocks++;
    }
    free(temp);
}

/* --Main()
 * Purpose: Main flow of the program
*/
int main(int argc, char **argv)
{
    //get all the information from the super block
    char *p = getAllSuperBlock(argv[1]);

    //get allocated blocks, reservered blocks, free blocks in FAT
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
