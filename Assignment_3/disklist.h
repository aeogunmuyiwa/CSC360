// Directory Entries
struct dir_entries_t {
    char file_type;
    int file_size;
    char *file_name;
    char *date_Modified;
} PACKED;
typedef struct dir_entries_t dir_entries_t;
