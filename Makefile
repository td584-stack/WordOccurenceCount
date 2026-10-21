CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -g

words: src/word.c src/word.h
	$(CC) $(CFLAGS) -o word src/word.c

clean:
	rm -f word
