// #include "userInput.h"
#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#define STRINGWIDTH 5

void print_board(int **gameBoard, int numrows, int numcolumns)
{
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcolumns; col++)
        {
            // %-* width adds 
            printw("%-*d", STRINGWIDTH, gameBoard[row][col]);
        }
        printw("\n");
    }
}

int main()
{	
    srand((unsigned int)time(NULL));
    int **gameBoard = create2dArr(MAX_ROWS, MAX_COLUMNS);
    reset2dArr(gameBoard, MAX_ROWS, MAX_COLUMNS);
	int choice = 0;
	int c;
    bool quit = false;
    bool validChoice = true;
	initscr();
	clear();
		
	
	while(!quit)
	{	
        if (validChoice)
        {
            clear();
            printw("Use wasd to choose move, r to reset and q to quit\n");
            addRandomSquare(gameBoard, 4, 4);
            //print2dArr(gameBoard, 4, 4);
            print_board(gameBoard, 4, 4);
            //printw("hello world");
            refresh();
        }
        
        c = getch();
        validChoice = true;
		switch(c)
		{	
            case 'w':
				squashUp(gameBoard, 4, 4);
				break;
			case 's':
				squashDown(gameBoard, 4, 4);
				break;
			case 'a':
				squashLeft(gameBoard, 4, 4);
				break;
            case 'd':
                squashRight(gameBoard, 4, 4);
                break;
            case 'q':
                printf("You have chosen to quit the game.");
                quit = true;
                break;
            case 'r':
                reset2dArr(gameBoard, 4, 4);
                break;
			default:
				printw("Invalid charcter pressed is = %3d -> '%c'\n", c, c);
                validChoice = false;
				break;
		}
	}
	clrtoeol();
	refresh();
	endwin();
    free2dArr(gameBoard, MAX_ROWS);
    free(gameBoard);
	return 0;
}


