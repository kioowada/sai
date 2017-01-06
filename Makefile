all:
	make dice.o
	make board.o
	make event.o
	make list.o

dice.o: dice.c dice.h
	gcc dice.c -o dice.o -c

board.o: board.c board.h
	gcc board.c -o board.o -c

event.o: event.c event.h
	gcc event.c -o event.o -c

list.o: list.c list.h
	gcc list.c -o list.o -c

test: dice.o board.o test.c
	gcc test.c dice.o board.o -o test
