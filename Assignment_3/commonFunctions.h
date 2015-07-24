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

int getSuperBlockInfo(char *mmap, int offset, int length);
int getBlockSize(char *mmap);
void checkAllocation(char *temp);
