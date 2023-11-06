#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100


struct wordCounts {
    char word[100];  
    int count;
};

int main() {


char buffer[1024];
ssize_t bytes_read;  // number of characters/bytes read 
int word_count = 0;
int store_word[1000]; //store words in array
int unique_word_count = 0;
int fd;
struct wordCounts word_counts[MAX_LENGTH];



fd = open("sentence.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

while ((bytes_read = read(fd, buffer, sizeof(buffer)) > 0)) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            char c = buffer[i];

            if (word_length >= MAX_LENGTH || c == ' ' || !isalnum(c)) {
                if (word_length > 0) {
                    // Null-terminate the word
                    store_word[word_length] = '\0';


                }

                }
        }
}

   return 0;
}
