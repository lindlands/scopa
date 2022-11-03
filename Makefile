CFLAGS = 

all: scopa clean

scopa: scopa.c list.o command.o macros.h
	gcc scopa.c list.o command.o -o scopa $(CFLAGS)

list.o: list.c list.h macros.h
	gcc -c list.c -o list.o $(CFLAGS)

command.o: command.c command.h macros.h
	gcc -c command.c -o command.o $(CFLAGS)

clean:
	rm -f *.o
