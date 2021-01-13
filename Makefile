CC = gcc
FLAGS = -Wall -g

all: frequency

main: frequency.c
	$(CC) $(FLAGS) frequency.c -o frequency
	
.PHONY:  frequency clean all
	
clean:
	rm -f *.o frequency	
	
