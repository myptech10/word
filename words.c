#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>


typedef struct _Node {
    char* word;
    int freq;
    struct _Node* next;
} Node;

#define HASH_TABLE_SIZE 100 // not sure if this enough 
static Node* hash_table[HASH_TABLE_SIZE] = {NULL};
static int anyValidFiles = 0;
static int anyWordsRead = 0;
 

//HASH FUNCTION
unsigned int hash(char* word) {
    unsigned int hash = 0;
    while (*word) {
        hash = (hash * 31) + *word; //convert into hash code  
        word++;
    }
    return hash % HASH_TABLE_SIZE;
}

void insertHash(char* word) {
    anyWordsRead = 1;

    unsigned int index = hash(word);

    // Search word in LL at index
    Node* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->freq++;
            return;
        }
        current = current->next;
    }
    //create new if not fond 
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->word = strdup(word);
    newNode->freq = 1;
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
}

void displaySortedWords() {
    int anyNodesLeft = 1;
    Node* current;

    // while(anyNodesLeft) {
        
    // }

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Node* current = hash_table[i];
        while (current != NULL) {
            char buffer[1024];
            int len = snprintf(buffer, sizeof(buffer), "%s -> %d\n", current->word, current->freq);
            write(STDOUT_FILENO, buffer, len);
            //printf("%s -> %d\n", current->word, current->freq);
            current = current->next;
        }
    }
}

int fileExists(char* file_name) {
    if (access(file_name, F_OK) == 0) {
        return 1;
    } else {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error opening %s, %s\n", file_name, strerror(errno));
        write(2, error_message, len);
        return 0;
    }
}



int isDirectory(char* file_name) {
    struct stat file_buffer;

    if (stat(file_name, &file_buffer) != 0) {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error checking file type for %s: %s\n", file_name, strerror(errno));
        write(2, error_message, len); //2 - Standard Error (stderr)
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

    //fprintf(stderr, "Error opening %s, not a .txt file\n", file_name);
    return 0;
}

int hasReadPerms(char* file_name) {
    if (access(file_name, R_OK) == 0) {
        return 1;
    } else {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error: Cannot read %s: %s\n", file_name, strerror(errno));
        write(2, error_message, len);
        errno = 0; // Reset the errno value
        return 0;
    }
}

int count_words(char* file_name) {
    anyValidFiles = 1;

    char buffer[1024];
    int fd;
    ssize_t bytes_read;  // number of characters/bytes read 
    //char store_word[100]; //store words in array
    //char* store_word = NULL; // initializes a pointer variable store_word to NULL
    char* store_word = (char*)malloc(1024 * sizeof(char));

    fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    int wordIndex = 0;

    while ((bytes_read = read(fd, buffer, 1024)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            char separators[] = {' ', '.', '\n', '\t', ';', '!', ':', '?', ',', '"', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '@', '#', '$', '%', '^', '&', '*', '<', '>', '[', ']', '|', '/', '\\', '{', '}'};
            int isSeparator = 0;

            for(int j = 0; j < sizeof(separators)/sizeof(char); j++) {
                if(buffer[i] == separators[j]) {
                    isSeparator = 1;
                }
            }
                    
            if (!isSeparator) {
                //printf( "%c",  buffer[i]); // debug
                if (buffer[i] == '-') {
                    if(i != bytes_read - 1 && buffer[i+1] == '-') {
                        i++;
                    }
                    
                    store_word[wordIndex] = '\0'; // Null-terminate the word
                    insertHash(store_word);
                    //printf("Wordssss: %s\n", store_word); // debug
                    wordIndex = 0;
                } else if(isdigit(buffer[i])) {
                    if(wordIndex==0) {
                        store_word[wordIndex] = '\0'; 
                    } else{
                        insertHash(store_word);
                        // printf("Word: %s\n", store_word); debug
                    }
                        wordIndex = 0;
                        continue;
                    }

                else{
                //store character in array
                 store_word = (char*)realloc(store_word, wordIndex + 2);  
                store_word[wordIndex] = buffer[i];
                store_word[wordIndex + 1] = '\0'; 
                wordIndex++;     
                } 
            } 
            
            else { //a weird character 
                if (wordIndex > 0) {
                    store_word[wordIndex] = '\0'; // Null-terminate the word
                    insertHash(store_word);
                    wordIndex = 0;
                }
            }
        }
 
    }


    if (wordIndex > 0) {
        store_word[wordIndex] = '\0';
        // printf("Word: %s\n", store_word);
        insertHash(store_word);
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
                    //printf("dir %s\n", current_file->d_name); // debug
                    recurseDirectory(current_file->d_name);
                } else if (isTextFile(current_file->d_name) && hasReadPerms(current_file->d_name)) {
                    printf("%s\n", current_file->d_name); // debug
                    count_words(current_file->d_name);
                }
            }
        } else {
            if (errno != 0) {
                fprintf(stderr, "Error traversing directory %s: %s\n", file_name, strerror(errno));
                errno = 0;
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
                //printf("dir %s\n", argv[i]); // debug
                recurseDirectory(argv[i]);
            } else if (hasReadPerms(argv[i])) {
                printf("%s\n", argv[i]); // debug
                count_words(argv[i]);
            }
        }
    }

    //debug
    // char buf[200];
    // printf("%s\n", getcwd(buf, 200));

    printf("\n");


    if(anyValidFiles) {
        if (anyWordsRead) {
            // sort and print output
            displaySortedWords();
            return 0;
        } else {
            const char* error_message = "Error: empty file, no words to process\n";
            write(STDERR_FILENO, error_message, strlen(error_message));        }
    } else {
       const char* error_message = "Error: No valid arguments to process\n";
       write(STDERR_FILENO, error_message, strlen(error_message));
    }
    return -1;
}
