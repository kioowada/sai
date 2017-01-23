C_FLAG=-g

all:
	make dice.o
	make board.o
	make event.o
	make list.o
	make game.o


game.o: game.c game.h
	gcc game.c -o game.o -c $(C_FLAG)

dice.o: dice.c dice.h
	gcc dice.c -o dice.o -c $(C_FLAG)

board.o: board.c board.h
	gcc board.c -o board.o -c $(C_FLAG)

event.o: event.c event.h
	gcc event.c -o event.o -c $(C_FLAG)

list.o: list.c list.h
	gcc list.c -o list.o -c $(C_FLAG)

test: game.o dice.o board.o list.o event.o test.c
	gcc test.c event.o dice.o board.o game.o list.o -o test $(C_FLAG)
