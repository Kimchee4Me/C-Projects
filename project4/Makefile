CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra

all: public01.x public02.x public03.x public04.x public05.x student_tests.x

public01.x: public01.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o public01.o -o public01.x

public02.x: public02.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o public02.o -o public02.x

public03.x: public03.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o public03.o -o public03.x

public04.x: public04.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o public04.o -o public04.x

public05.x: public05.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o public05.o -o public05.x

student_tests.x: student_tests.o calendar.o my_memory_checker_216.o
	$(CC) calendar.o my_memory_checker_216.o student_tests.o -o student_tests.x

public01.o: public01.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) public01.c

public02.o: public02.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) public02.c

public03.o: public03.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) public03.c

public04.o: public04.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) public04.c

public05.o: public05.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) public05.c

student_tests.o: student_tests.c calendar.h my_memory_checker_216.h
	$(CC) -c $(CFLAGS) student_tests.c

calendar.o: calendar.c calendar.h
	$(CC) -c $(CFLAGS) calendar.c

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) -c $(CFLAGS) my_memory_checker_216.c
clean:
	rm -f *.x *.o
