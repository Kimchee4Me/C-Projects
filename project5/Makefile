CC = gcc
CFLAGS = -ansi -Wall -g -O0  -Wwrite-strings -pedantic-errors -Wshadow
all: d8sh

lexer.o: lexer.c lexer.h
	$(CC) -c $(CFLAGS) lexer.c

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) -c $(CFLAGS) parser.tab.c

executor.o: executor.c executor.h command.h
	$(CC) -c $(CFLAGS) executor.c

d8sh.o: d8sh.c executor.h lexer.h
	$(CC) -c $(CFLAGS) d8sh.c

d8sh: d8sh.o lexer.o executor.o parser.tab.o
	$(CC) lexer.o executor.o parser.tab.o d8sh.o -lreadline -o d8sh 


