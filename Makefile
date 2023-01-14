CC = gcc
CFLAGS = -g -Wall

all: cshell

cshells: cshell.o
	$(CC) $(CFLAGS) cshell.o -o cshell

clean:
	rm -f cshell cshell.o