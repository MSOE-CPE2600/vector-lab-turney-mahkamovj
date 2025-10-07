CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJS = main.o vector.o
EXEC = test

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

main.o: main.c vector.h
	$(CC) $(CFLAGS) -c main.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

clean:
	rm -f $(OBJS) $(EXEC)