all:
	make dice.o
	make board.o

dice.o: dice.c dice.h
	gcc dice.c -o dice.o -c

board.o: board.c board.h
	gcc board.c -o board.o -c

test: dice.o board.o test.c
	gcc test.c dice.o board.o -o test
