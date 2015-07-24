#include "commonFunctions.h"


struct superblock_t sb_t;

char *find_directory_entry(char *mmap, filename){
    char *dir_entry = (char *)malloc()
}

void get_file(char *mmap, char *filename){
    FILE * new_file = fopen(filename, "wb");
    entry = find_directory_entry(mmap, filename);
}

int main(int argc, char **argv)
{
    char *copy_file = argv[2];
    char *destination_file = argv[3];

    char *mmap = getAllSuperBlock(argv[1]);

    get_file(mmap, filename);
}
