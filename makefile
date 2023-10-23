all: main

main: main.o libs
	gcc main.o -o main -l ncurses -L. game.a userInput.a

main.o: main.c
	gcc -c main.c -o main.o

server: server.o libs
	gcc server.o -o server -I /opt/homebrew/opt/mysql/include/mysql/ -L /opt/homebrew/opt/mysql/lib ./game.a ./database.a -l ncurses -l mysqlclient

server.o: server.c
	gcc -c server.c -o server.o

client: client.o libs
	gcc client.o -o client -l ncurses -L. game.a userInput.a shared.a

client.o: client.c
	gcc -c client.c -o client.o

userInput.o: userInput.c
	gcc -c userInput.c -o userInput.o

userInput.a: userInput.o
	ar rcs userInput.a userInput.o

game.o: game.c
	gcc -c game.c -o game.o

game.a: game.o
	ar rcs game.a game.o

shared.o: shared.c
	gcc -c shared.c -o shared.o

shared.a: shared.o
	ar rcs shared.a shared.o

database.o: database.c
	gcc -c database.c -o database.o

database.a: database.o
	ar rcs database.a database.o

libs: game.a userInput.a shared.a database.a

clean:
	rm -f main server client database *.o *.a