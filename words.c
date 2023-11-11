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
    int anyNodesLeft = 0;
    Node* current;

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        current = hash_table[i];
        while (current != NULL) {
            char buffer[sizeof(current->word)+sizeof(int)+10];
            int len = snprintf(buffer, sizeof(buffer), "%s -> %d\n", current->word, current->freq);
            write(STDOUT_FILENO, buffer, sizeof(current->word)+sizeof(int)+5);
            current = current->next;            
        }
    }


    // //printf("%s -> %d\n", current->word, current->freq);
    // current = current->next;

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

// int terminate_word(char* store_word, int* ptr_word_index) {

// }

char* save_word(char* word_buffer, int* p_word_index, int* p_size_word_buffer) {
    word_buffer[*p_word_index] = '\0';
    insertHash(word_buffer);
    free(word_buffer);
    *p_word_index = 0;
    *p_size_word_buffer = 10;
    return malloc(*p_size_word_buffer);
}

void count_words(char* file_name) {
    anyValidFiles = 1;

    char separators[] = {' ', '.', '\n', '\t', ';', '!', ':', '?', ',', '"', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '@', '#', '$', '%', '^', '&', '*', '<', '>', '[', ']', '|', '/', '\\', '{', '}'};
    int fd;
    char char_buffer;
    int size_word_buffer = 10;
    int wordIndex = 0;
    char* word_buffer = malloc(size_word_buffer);
    enum char_type {NOTHING, HYPHEN, REGULAR, APOSTROPHE, DELIMITER};
    enum char_type prev = NOTHING;

    fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
    }

    while (1) {
        int status = read(fd, &char_buffer, 1);
        if (status > 0) {
            int isSeparator = 0;
            for(int i = 0; i < sizeof(separators)/sizeof(char); i++) {
                if(char_buffer == separators[i]) {
                    isSeparator = 1;
                }
            }
            //printf("%c, isSeparator: %d\n", char_buffer, isSeparator); // debug

            if(!isSeparator) {
                if(wordIndex == size_word_buffer - 2) {
                    size_word_buffer *= 2;
                    word_buffer = realloc(word_buffer, size_word_buffer);
                }
                if(char_buffer == '-') {
                    if(prev == HYPHEN) {
                        if(wordIndex > 0) {
                            wordIndex--;
                            word_buffer = save_word(word_buffer, &wordIndex, &size_word_buffer);
                        }
                        prev = DELIMITER;
                    } else if (prev == DELIMITER || prev == NOTHING) {
                        prev = HYPHEN;
                    } else if (prev == REGULAR) {
                        word_buffer[wordIndex] = char_buffer;
                        wordIndex++;
                        prev = HYPHEN;
                    } else if (prev == APOSTROPHE) {
                        word_buffer[wordIndex] = char_buffer;
                        wordIndex++;
                        prev = HYPHEN;
                    }
                } else if (char_buffer == '\'') {
                    if(prev == HYPHEN) {
                        while(wordIndex > 0 && word_buffer[wordIndex - 1] == '-') {
                            wordIndex--;
                        }
                        if(wordIndex > 0) {
                            word_buffer = save_word(word_buffer, &wordIndex, &size_word_buffer);
                        }
                    }
                    word_buffer[wordIndex] = char_buffer;
                    wordIndex++;
                    prev = APOSTROPHE;
                } else {
                    word_buffer[wordIndex] = char_buffer;
                    wordIndex++;
                    prev = REGULAR;
                }
            } else if (isSeparator && wordIndex > 0){
                if(prev == HYPHEN) {
                    while(wordIndex > 0 && word_buffer[wordIndex - 1] == '-') {
                        wordIndex--;
                    }
                }
                word_buffer = save_word(word_buffer, &wordIndex, &size_word_buffer);
                prev = DELIMITER;
            }
        } else if (status == 0) {
            if(wordIndex > 0) {
                if(prev == HYPHEN) {
                    while(word_buffer[wordIndex - 1] == '-') {
                        wordIndex--;
                    }
                }
                word_buffer = save_word(word_buffer, &wordIndex, &size_word_buffer);
            }
            break;
        } else if (status < 0) {
            perror("read failed"); // change to write
        }
    }

    free(word_buffer);
    close(fd);
}

void recurseDirectory(char* file_name) {
    if (chdir(file_name) != 0) {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error: Cannot move into directory %s: %s\n", file_name, strerror(errno));
        write(STDERR_FILENO, error_message, len);
        return;
    }

    DIR* current_dir = opendir(".");
    if (current_dir == NULL) {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error: Cannot read from directory %s: %s\n", file_name, strerror(errno));
        write(STDERR_FILENO, error_message, len);
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
                    //debug
                    //char* buffer;
                    // int len = snprintf(buffer, sizeof(buffer), "dir %s\n", current_file->d_name);
                    // write(STDOUT_FILENO, buffer, len); // Replace printf with write
                    recurseDirectory(current_file->d_name);
                } else if (isTextFile(current_file->d_name) && hasReadPerms(current_file->d_name)) {
                    char buffer[sizeof(current_file->d_name)];
                    int len = snprintf(buffer, sizeof(buffer), "%s\n", current_file->d_name);
                    write(STDOUT_FILENO, buffer, len); // Replace printf with write
                    count_words(current_file->d_name);
                }
            }
        } else {
            if (errno != 0) {
                char error_message[256];
                int len = snprintf(error_message, sizeof(error_message), "Error traversing directory %s: %s\n", file_name, strerror(errno));
                write(STDERR_FILENO, error_message, len); 
                errno = 0;
            }
            break;
        }
    } while(1);

    if (closedir(current_dir) != 0) {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error: Cannot close directory %s: %s\n", file_name, strerror(errno));
        write(STDERR_FILENO, error_message, len);
    }

    if (chdir("..") != 0) {
        char error_message[256];
        int len = snprintf(error_message, sizeof(error_message), "Error: Cannot move out of directory %s: %s\n", file_name, strerror(errno));
        write(STDERR_FILENO, error_message, len);
    }
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        // debug
        // char* buffer;
        // int len = snprintf(buffer, sizeof(argv[i]), "Argument %d: %s\n", i, argv[i]);
        // write(STDOUT_FILENO, buffer, len); 

        if (fileExists(argv[i])) {
            if (isDirectory(argv[i]) && hasReadPerms(argv[i])) {
                // debug
                // char* dir_message;
                // int dir_len = snprintf(dir_message, sizeof(dir_message), "dir %s\n", argv[i]);
                // write(STDOUT_FILENO, dir_message, dir_len); 
                recurseDirectory(argv[i]);
            } else if (hasReadPerms(argv[i])) {
                char file_message[sizeof(argv[i])];
                int file_len = snprintf(file_message, sizeof(file_message), "%s\n", argv[i]);
                write(STDOUT_FILENO, file_message, file_len); 
                count_words(argv[i]);
            }
        }
    }

    //debug
    // char buf[200];
    // printf("%s\n", getcwd(buf, 200));

    printf("\n");

    if (anyValidFiles) {
        if (anyWordsRead) {
            // sort and print output
            printf("\n");
            displaySortedWords();
            return 0;
        } else {
            const char* error_message = "Error: empty file, no words to process\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
    } else {
       const char* error_message = "Error: No valid arguments to process\n";
       write(STDERR_FILENO, error_message, strlen(error_message));
    }
    return -1;
}

