#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

int main() {


char buffer[1024];
int fd;
ssize_t bytes_read;  // number of characters/bytes read 
int word_count = 0;
char store_word[100]; //store words in array
int word_length = 0;






fd = open("sentence.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

while ((bytes_read = read(fd, buffer, sizeof(buffer)) > 0)) {
        for (ssize_t i = 0; i < bytes_read; i++) { //for each word 
        
            char c = buffer[i];

        if (c == ' ' || !isalnum(c)) {
                if (word_length > 0) {
                    // Null-terminate the word
                    store_word[word_length] = '\0';
                    
                }
                else{
                   
                }
        }
}
        }
   return 0;
}


