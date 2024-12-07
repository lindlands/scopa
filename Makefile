CFLAGS = 

all: scopa clean

scopa: scopa.c score.o list.o command.o macros.h
	gcc scopa.c score.o list.o command.o view.o -o scopa $(CFLAGS)

score.o: score.c score.h list.c list.h macros.h
	gcc -c score.c -o score.o $(CFLAGS)

list.o: list.c list.h macros.h
	gcc -c list.c -o list.o $(CFLAGS)

command.o: command.c command.h macros.h
	gcc -c command.c -o command.o $(CFLAGS)

view.o: view.c view.h macros.h
	gcc -c view.c -o view.o $(CFLAGS)

clean:
	rm -f *.o
