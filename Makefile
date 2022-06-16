CC=gcc
CFLAGS=-g -Wall -std=c99
OBJS=trieexample.o
BIN=main
SUBMITNAME=project.zip

all:$(BIN)

main:$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

%o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r main *.o *.dSYM $(SUBMITNAME)

submit:
	$(RM) $(SUBMITNAME)
	zip $(SUBMITNAME) $(BIN)
