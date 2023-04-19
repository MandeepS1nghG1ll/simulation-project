#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define DISK_SIZE 1024
#define MAX_FILENAME_SIZE 32
#define MAX_FILES 32

int main() {
typedef struct {
    char name[MAX_FILENAME_SIZE];
    int start_block;
    int num_blocks;
} file_t;

typedef struct {
    int num_files;
    file_t files[MAX_FILES];
} directory_t;

char disk[DISK_SIZE][BLOCK_SIZE];
directory_t directory;
int num_free_blocks;
int free_block_list[DISK_SIZE];

void init_disk() {
    for (int i = 0; i < DISK_SIZE; i++) {
        free_block_list[i] = i;
    }
    num_free_blocks = DISK_SIZE;
}

void create_file(char* name, int size) {
    if (size > num_free_blocks) {
        printf("Error: not enough free space on disk.\n");
        return;
    }
    if (strlen(name) > MAX_FILENAME_SIZE) {
        printf("Error: filename too long.\n");
        return;
    }
    file_t new_file;
    strcpy(new_file.name, name);
    new_file.num_blocks = size;
    if (size == 1) {
        new_file.start_block = free_block_list[0];
        free_block_list[0] = -1;
    } else {
        new_file.start_block = free_block_list[0];
        for (int i = 0; i < size; i++) {
            free_block_list[i] = free_block_list[i+1];
        }
    }
  num_free_blocks -= size;
    directory.files[directory.num_files++] = new_file;
    printf("File '%s' created.\n", name);
}

void delete_file(char* name) {
    int file_index = -1;
    for (int i = 0; i < directory.num_files; i++) {
        if (strcmp(directory.files[i].name, name) == 0) {
            file_index = i;
            break;
        }
    }
    if (file_index == -1) {
        printf("Error: file '%s' not found.\n", name);
        return;
    }
    file_t deleted_file = directory.files[file_index];
    for (int i = deleted_file.start_block; i < deleted_file.sta>        free_block_list[i] = i;
    }
    num_free_blocks += deleted_file.num_blocks;
    for (int i = file_index; i < directory.num_files-1; i++) {
        directory.files[i] = directory.files[i+1];
    }
    directory.num_files--;
    printf("File '%s' deleted.\n", name);
}

void rename_file(char* old_name, char* new_name) {
 for (int i = 0; i < directory.num_files; i++) {
        if (strcmp(directory.files[i].name, old_name) == 0) {
            if (strlen(new_name) > MAX_FILENAME_SIZE) {
                printf("Error: new filename too long.\n");
                return;
            }
            strcpy(directory.files[i].name, new_name);
            printf("File '%s' renamed to '%s'.\n", old_name, ne>
            return;
        }
    }
    printf("Error: file '%s' not found.\n", old_name);
}

void move_file(char* name, int new_start_block) {
    int file_index = -1;
    for (int i = 0; i < directory.num_files; i++) {
        if (strcmp(directory.files[i].name, name) == 0) {
            file_index = i;
            break;
        }
    }
    if (file_index == -1) {
        printf("Error: file '%s' not found.\n", name);
        return;
    }
    file_t moved_file = directory.files[file_index];
    if (new_start_block + moved_file.num_blocks > DISK_SIZE) {
        printf("Error: new start block out of range.\n");
        return;
  }
    if (new_start_block < moved_file.start_block) {
        // move file to lower start block
        for (int i = new_start_block; i < moved_file.start_bloc>            free_block_list[i + moved_file.num_blocks] = free_b>        }
        moved_file.start_block = new_start_block;
    } else if (new_start_block > moved_file.start_block) {
        // move file to higher start block
        for (int i = moved_file.start_block + moved_file.num_bl>            free_block_list[i] = free_block_list[i - moved_file>        }
        moved_file.start_block = new_start_block;
    } else {
        // file is already at the specified start block
        return;
    }
    directory.files[file_index] = moved_file;
    printf("File '%s' moved to start block %d.\n", name, new_st>}
return 0 ;
}

