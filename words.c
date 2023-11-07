#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct _Node {
    char* word;
    int freq;
    struct _Node* next;
} Node;

#define ARRAY_LENGTH 100
//static Node* map_of_words[ARRAY_LENGTH];

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

int main(int argc, char* argv[]) {
    for(int i = 1; i < argc; i++){
        printf("Argument %d: %s\n", i, argv[i]); //debug
        
        struct stat file_buffer;
        int status = stat(argv[i], &file_buffer);

        if (status == 0) {  // does the file exist
            if (S_ISDIR(file_buffer.st_mode)) { // is it a directory
                printf("is directory\n");
            } else {
                if (strlen(argv[i]) > 4) { // is it long enough to be a .txt
                    char extension[5];
                    memcpy(extension, argv[i]+(strlen(argv[i])-4), 5);
                    printf("%s\n", extension); // debug

                    if (strcmp(extension, ".txt") == 0) {   // is it a .txt
                        printf("is text file\n");
                        
                    } else {
                        fprintf(stderr, "Error opening %s, not a .txt file\n", argv[i]);
                    }
                } else {
                    fprintf(stderr, "Error opening %s, not a .txt file\n", argv[i]);
                }                
            }
        } else {
            fprintf(stderr, "Error opening %s, %s\n", argv[i], strerror(errno));
        }
    }

    return 0;
}





int count_words(){
    char buffer[1024];
    int fd;
    ssize_t bytes_read;  // number of characters/bytes read 
    //int word_count = 0;
    //char store_word[100]; //store words in array
    //int word_length = 0;


    fd = open("sentence.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer)) > 0)) {
        for (ssize_t i = 0; i < bytes_read; i++) { //for each word 
        
            char c = buffer[i];

            // if (c == ' ' || !isalnum(c)) {
            //         if (word_length > 0) {
            //             // Null-terminate the word
            //             store_word[word_length] = '\0';
                        
            //         }
            //         else{
                    
            //         }
            // }

            printf("%d", c);
        }
    }

    return 0;
}