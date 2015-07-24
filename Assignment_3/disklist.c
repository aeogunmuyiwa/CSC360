#include "disklist.h"
#include "commonFunctions.h"


/* --calculateModifiedTime()
 * Purpose: extract time pieces from the modify time char *
 *          combine all integer pieces into a char * and return
*/
char *calculateModifiedTime(char *modify_time)
{
    //get each individual part of the modified time
    int year = (modify_time[0]<<8) + (modify_time[1] & 0xFF);
    int month = modify_time[2];
    int day = modify_time[3];
    int hour = (modify_time[4] & 0xFF);
    int minute = modify_time[5];
    int second = modify_time[6];

    //combine all parts into a char *
    sprintf(modify_time, "%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    return modify_time;
}

/* --getFileList();
 * Purpose: get the list of files in a Directory
*/
void listDirectoryEntries(char *mmap, int offset, int length, int num_dir_blocks, int block_size)
{
    int i, j, k;

    char *dir_entry = (char *)malloc(sizeof(char) * length);
    char *file_name = (char *)malloc(sizeof(char) * FILE_NAME_SIZE);
    unsigned char *file_size_bytes = (unsigned char *)malloc(sizeof(unsigned char) * FILE_SIZE);
    char file_type;
    char *modify_time = (char *)malloc(sizeof(char) * MODIFY_TIME_SIZE);

    int file_size = 0;
    printf("\n");
    for (i = 0; i < num_dir_blocks; i++)
    {
        for (j = 0; j < 8; j++)
        {
            //get directory entry char* from mmap
            memcpy(dir_entry, mmap+offset+block_size*i+length*j, length);

            //determine whether entry is a file or a directory
            if((dir_entry[0] & 0x02) == 0x02)
            {
                file_type = 'F';
            }
            else if((dir_entry[0] & 0x04) == 0x04)
            {
                file_type = 'D';
            }
            else break;

            //get file_name and file_size_bytes from dir_entry
            memcpy(file_name, dir_entry + FILE_NAME_START, FILE_NAME_SIZE);
            memcpy(file_size_bytes, dir_entry + 9, FILE_SIZE);

            //count size of file
            file_size = 0;
            for(k = 0; k < FILE_SIZE; k++)
            {
                file_size += ((int)file_size_bytes[k]<<(8*(-k + FILE_SIZE -1)));
            }
            //determine date last modified
            memcpy(modify_time, dir_entry + MODIFY_TIME_START, MODIFY_TIME_SIZE);
            modify_time = calculateModifiedTime(modify_time);

            //output information
            printf("%c %10d %30s %s\n", file_type, file_size, file_name, modify_time);
        }
    }
    printf("\n");

    //release allocated memory
    free(dir_entry);
    free(file_name);
    free(file_size_bytes);
    free(modify_time);
}


/* --Main()
 * Purpose: Controls the main flow of the program
*/

int main(int argc, char **argv)
{
    char *filename = argv[1];
    // char *subdir = argv[2];
    char *p;
    struct stat stats;

    //open disk file for reading
    int open_file = open(filename, O_RDONLY);
    if(open_file == -1)
    {
        printf("[ERROR]: Failed to open disk image: %s\n",filename);
        exit(-1);
    }
    //get mmap
    fstat(open_file, &stats);
    p = mmap(NULL, stats.st_size, PROT_READ, MAP_SHARED, open_file, 0);

    //get needed information to get list Files
    int dir_start_block = getSuperBlockInfo(p, 22, 4);
    int dir_block_count = getSuperBlockInfo(p, 26, 4);
    int block_size = getBlockSize(p);

    //get directory entries
    listDirectoryEntries(p, dir_start_block*block_size, SIZE_DIR_ENTRY, dir_block_count, block_size);
}
