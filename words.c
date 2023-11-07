#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>

// do we really need all of these?
// #include <stdlib.h>
// #include <ctype.h>

typedef struct _Node {
    char* word;
    int freq;
    struct _Node* next;
} Node;

#define ARRAY_LENGTH 100
static Node* map_of_words[ARRAY_LENGTH];
static int anyValidFiles = 0;

// write desc later
static int hash(char* word) {
    int length = 5;
    if(strlen(word) < 5) {
        length = strlen(word);
    }

    int ascii_values_sum = 0;

    for(int i = 0; i < length; i++){
        ascii_values_sum += word[i];
    }

    ascii_values_sum /= ARRAY_LENGTH;
    return ascii_values_sum;
}

int fileExists(char* file_name) {
    if (access(file_name, F_OK) == 0) {
        return 1;
    } else {
        fprintf(stderr, "Error opening %s, %s\n", file_name, strerror(errno));
        return 0;
    }
}

int isDirectory(char* file_name) {
    struct stat file_buffer;

    if (stat(file_name, &file_buffer) != 0){
        fprintf(stderr, "Error checking file type for %s: %s\n", file_name, strerror(errno));
        return 0;
    }
 
    if (S_ISDIR(file_buffer.st_mode)) {
        return 1;
    } else {
        return 0;
    }
}

int isTextFile(char* file_name) {
    if(strlen(file_name) > 4) {
        char extension[5];
        memcpy(extension, file_name+strlen(file_name)-4, 5);

        if (strcmp(extension, ".txt") == 0) {
            return 1;
        }
    }

    fprintf(stderr, "Error opening %s, not a .txt file\n", file_name);
    return 0;
}

int hasReadPerms(char* file_name) {
    if(access(file_name, R_OK) == 0){
        return 1;
    }else{
        fprintf(stderr, "Error: Cannot read %s: %s\n", file_name, strerror(errno));
        return 0;
    }
}

int count_words(char* fileName) {
    anyValidFiles = 1;

    char buffer[1024];
int fd;
ssize_t bytes_read;  // number of characters/bytes read 
int word_count = 0;
char store_word[100]; //store words in array
char store_dash[100]; //store words in array

int word_length = 0;



fd = open("sentence.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

int wordIndex = 0;
int dash = 0;
int wordDone = 0;

  while ((bytes_read = read(fd, buffer, 1024)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '!' && buffer[i] != '?' && buffer[i] != ',' && buffer[i] != '"' && buffer[i] != '.')  {
                //printf( "%c",  buffer[i]);
                if ((buffer[i] == '-' && buffer[i+1] == '-')){
                    i++;
                    dash = 2;
                    store_word[wordIndex] = '\0'; // Null-terminate the word
                    printf("Wordssss: %s\n", store_word);
                    wordIndex = 0;

 
                    }
                else if(isdigit(buffer[i])){
                        if(wordIndex==0){
                        store_word[wordIndex] = '\0'; 
    
                        } else{
                         printf("Word: %s\n", store_word); }
                         wordIndex = 0;
                         continue;
                }

                else{
                //store character in array
                store_word[wordIndex] = buffer[i];
                wordIndex++;     
                } 
            } 
            
            else { //a weird character 
                if (wordIndex > 0) {
                    store_word[wordIndex] = '\0'; // Null-terminate the word
                    printf("Word: %s\n", store_word);
                    wordIndex = 0;
                }
            }
        }
    }

 close(fd);
   return 0;

}

void recurseDirectory(char* file_name) {   
    if (chdir(file_name) != 0) {
        fprintf(stderr, "Error: Cannot move into directory %s: %s\n", file_name, strerror(errno));
        return;
    }

    DIR* current_dir = opendir(".");
    if (current_dir == NULL) {
        fprintf(stderr, "Error: Cannot read from directory %s: %s\n", file_name, strerror(errno));
        return;
    }
    
    struct dirent* current_file;
    do {
        current_file = readdir(current_dir);
        if (current_file != NULL) {
            if (strcmp(current_file->d_name, ".") == 0 || strcmp(current_file->d_name, "..") == 0) {
                continue;
            } else {
                if (isDirectory(current_file->d_name) && hasReadPerms(current_file->d_name)) {
                    printf("dir %s\n", current_file->d_name); // debug
                    recurseDirectory(current_file->d_name);
                } else if (isTextFile(current_file->d_name) && hasReadPerms(current_file->d_name)) {
                    printf("%s\n", current_file->d_name); // debug
                    count_words(current_file->d_name);
                }
            }
        } else {
            if (errno != 0) {
                fprintf(stderr, "Error traversing directory %s: %s\n", file_name, strerror(errno));
                return;
            }
            break;
        }
    } while(1);


    if(closedir(current_dir) != 0){
        fprintf(stderr, "Error: Cannot close directory %s: %s\n", file_name, strerror(errno));
    }
    if(chdir("..") != 0) {
        fprintf(stderr, "Error: Cannot move out of directory %s: %s\n", file_name, strerror(errno));
    }

    return;
}

int main(int argc, char* argv[]) {
    for(int i = 1; i < argc; i++){
        //printf("Argument %d: %s\n", i, argv[i]); //debug
        if (fileExists(argv[i])) {
            if (isDirectory(argv[i]) && hasReadPerms(argv[i])) {
                printf("dir %s\n", argv[i]); // debug
                recurseDirectory(argv[i]);
            } else if (isTextFile(argv[i]) && hasReadPerms(argv[i])) {
                printf("%s\n", argv[i]); // debug
                count_words(argv[i]);
            }
        }
    }

    //debug
    // char buf[200];
    // printf("%s\n", getcwd(buf, 200));



    if(anyValidFiles) {
        // sort and print output
        return 0;
    } else {
        fprintf(stderr, "Error: No valid arguments to process\n");
        return -1;
    }
}
