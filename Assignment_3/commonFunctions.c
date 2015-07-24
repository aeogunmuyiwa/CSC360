
#include "commonFunctions.h"

struct superblock_t sb_t;

void checkAllocation(char *temp)
{
    if(temp == NULL)
    {
        printf("[ERROR]: Failed to allocate memory in function readFATInfo()");
        exit(-1);
    }
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

char *getAllSuperBlock(char *filename)
{
    struct stat stats;
    char *p;

    int open_file = open(filename, O_RDONLY);
    if(open_file == -1)
    {
        printf("[ERROR]: %s was not found\n", filename);
        exit(-1);
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

    return p;
}
