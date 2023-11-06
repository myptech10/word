CC = gcc
CFLAGS = -g -Wall -Wvla -fsanitize=address,undefined
# when finished with implmenting add -Werror back
CLEAN = rm *.o

words: words.o
	$(CC) $(CFLAGS) words.o -o words
	$(CLEAN)

words.o: words.c
	$(CC) $(CFLAGS) -c words.c