all: main

main: main.o libs
	gcc main.o -o main -l ncurses -L. game.a userInput.a

main.o: main.c
	gcc -c main.c -o main.o

userInput.o: userInput.c
	gcc -c userInput.c -o userInput.o

userInput.a: userInput.o
	ar rcs userInput.a userInput.o

game.o: game.c
	gcc -c game.c -o game.o

game.a: game.o
	ar rcs game.a game.o

libs: game.a userInput.a

clean:
	rm -f main *.o *.a