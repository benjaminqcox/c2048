#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include "userInput.h"
#include "game.h"

#define STRING_WIDTH 5

void printGame(game_t *game)
{
    printw("Score: %d, Turns: %d\n", game->score, game->num_turns);
    for (int row = 0; row < game->num_rows; row++)
    {
        for (int col = 0; col < game->num_columns; col++)
        {
            //printw("(%d, %d)", row, col);
            // %-* width adds 
            printw("%-*d", STRING_WIDTH, game->board[row][col]);
        }
        printw("\n");
    }
}

void printGame2(game_t game)
{
    printw("Score: %d, Turns: %d\n", game.score, game.num_turns);
    for (int row = 0; row < game.num_rows; row++)
    {
        for (int col = 0; col < game.num_columns; col++)
        {
            // %-* width adds 
            printw("%-*d", STRING_WIDTH, game.board[row][col]);
        }
        printw("\n");
    }
}

void free2dArr(int **arr, int num_rows)
{
    for (int row = 0; row < num_rows; row++)
    {
        free(arr[row]);
        arr[row] = NULL;
    }
}

void freeGame(game_t *game)
{
    // call free 2d array and then clear all stored variables in game (if there are any)
}

void print2dArr(game_t *game)
{
    printf("##########\n");
    for (int row = 0; row < game->num_rows; row++)
    {
        for (int col = 0; col < game->num_columns; col++)
        {
            if (col == game->num_columns-1)
            {
                printf("%d\n", game->board[row][col]);
            }
            else
            {
                printf("%d, ", game->board[row][col]);
            }   
        }
    }
    printf("##########\n");
}

int **create2dArr(int num_rows, int num_columns)
{
    // Allocate memory for 2d array
    int **new2dArr = (int**)malloc(num_rows * sizeof(int*));
    // Check memory was allocated correctly for the rows
    if (new2dArr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for rows.");
        exit(EXIT_FAILURE);
    }
    
    for (int row = 0; row < num_rows; row++)
    {
        // Allocate memory for each column in the array
        new2dArr[row] = (int *)malloc(num_columns * sizeof(int));
        if (new2dArr[row] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for columns.");
            // Free the memory assigned for the 2d array
            free2dArr(new2dArr, row);
            // Free the reference to the array
            free(new2dArr);
            new2dArr = NULL;
            exit(EXIT_FAILURE);
        }
    }
    return new2dArr;
}

void reset2dArr(int **arr, int num_rows, int num_columns)
{
    for (int row = 0; row < num_rows; row++)
    {
        for (int col = 0; col < num_columns; col++)
        {
            arr[row][col] = 0;
        }
    }
}

void resetGame(game_t *game)
{
    reset2dArr(game->board, game->num_rows, game->num_columns);
    game->score = 0;
    game->num_turns = 0;
}

game_t *makeGame(int num_rows, int num_columns)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    if (game == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for game.\n");
        exit(EXIT_FAILURE);
    }
    game->board = create2dArr(MAX_ROWS, MAX_COLUMNS);
    game->num_rows = num_rows;
    game->num_columns = num_columns;
    resetGame(game);
    return game;
}

void squashLeft(game_t *game)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < game->num_rows; row++)
    {
        col = 0;
        while (col < game->num_columns)
        {
            for (int comb = col+1; comb < game->num_columns; comb++)
            {
                if (game->board[row][comb] > 0)
                {
                    if (game->board[row][col] == 0)
                    {
                        game->board[row][col] = game->board[row][comb];
                        game->board[row][comb] = 0;
                        col--;
                    }
                    else if(game->board[row][col] == game->board[row][comb])
                    {
                        game->board[row][col] += game->board[row][comb];
                        game->score += game->board[row][col];
                        game->board[row][comb] = 0;
                    }
                    break;
                }
            }
            col++;
        }
    }
}

void squashRight(game_t *game)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = game->num_rows-1; row >= 0; row--)
    {
        col = game->num_columns-1;
        while (col > 0)
        {
            for (int comb = col-1; comb >= 0; comb--)
            {
                if (game->board[row][comb] > 0)
                {
                    if (game->board[row][col] == 0)
                    {
                        game->board[row][col] = game->board[row][comb];
                        game->board[row][comb] = 0;
                        col++;
                    }
                    else if(game->board[row][col] == game->board[row][comb])
                    {
                        game->board[row][col] += game->board[row][comb];
                        game->score += game->board[row][col];
                        game->board[row][comb] = 0;
                    }
                    break;
                }
            }
            col--;
        }
    }
}

void squashUp(game_t *game)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < game->num_rows; row++)
    {
        col = 0;
        while (col < game->num_columns)
        {
            for (int comb = col+1; comb < game->num_columns; comb++)
            {
                if (game->board[comb][row] > 0)
                {
                    if (game->board[col][row] == 0)
                    {
                        game->board[col][row] = game->board[comb][row];
                        game->board[comb][row] = 0;
                        col--;
                    }
                    else if(game->board[col][row] == game->board[comb][row])
                    {
                        game->board[col][row] += game->board[comb][row];
                        game->score += game->board[col][row];
                        game->board[comb][row] = 0;
                    }
                    break;
                }
            }
            col++;
        }
    }
}

void squashDown(game_t *game)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < game->num_rows; row++)
    {
        col = game->num_columns-1;
        while (col > 0)
        {
            for (int comb = col-1; comb >= 0; comb--)
            {
                if (game->board[comb][row] > 0)
                {
                    if (game->board[col][row] == 0)
                    {
                        game->board[col][row] = game->board[comb][row];
                        game->board[comb][row] = 0;
                        col++;
                    }
                    else if(game->board[col][row] == game->board[comb][row])
                    {
                        game->board[col][row] += game->board[comb][row];
                        game->score += game->board[col][row];
                        game->board[comb][row] = 0;
                    }
                    break;
                }
            }
            col--;
        }
    }
}

int countEmpty(game_t *game)
{
    int numEmpty = 0;
    for (int row = 0; row < game->num_rows; row++)
    {
        for (int col = 0; col < game->num_columns; col++)
        {
            if (game->board[row][col] ==  0)
            {
                numEmpty++;
            }
        }
    }
    return numEmpty;
}

void addRandomSquare(game_t *game)
{
    int numEmpty = countEmpty(game);
    // Check if numEmpty == 0 meaning there are no spare squares available for use
    int r = rand() % numEmpty + 1;
    int emptyCount = 0;
    for (int row = 0; row < game->num_rows; row++)
    {
        for (int col = 0; col < game->num_columns; col++)
        {
            if (game->board[row][col] == 0 && ++emptyCount == r)
            {
                // Generate a random number between 1 and 100
                // if it is greater than 10, then the value is 2
                // otherwise the value is 4
                // 10% chance of being a 4 value
                game->board[row][col] = ((rand() + 1) % 100) > 10 ? 2 : 4;
                return;
            }
        }
    }
}

void playGame(game_t *game)
{
    srand((unsigned int)time(NULL));
    int choice = 0;
	int c;
    bool quit = false;
    bool validChoice = true;
	initscr();
	clear();
	addRandomSquare(game);	
	
	while(!quit)
	{	
        clear();
        printw("Use wasd or arrow keys to choose move, r to reset and q to quit\n");
        printGame(game);
        refresh();
        c = getch();
        validChoice = true;
        if (c == 'w' || c == 65)
            squashUp(game);
        else if (c == 'a' || c == 68)
            squashLeft(game);
        else if (c == 's' || c == 66)
            squashDown(game);
        else if (c == 'd' || c == 67)
            squashRight(game);
        else if (c == 'r')
        {
            // Reset game, add square and loop back
            // continue needed to not increment the num turns
            resetGame(game);
            addRandomSquare(game);
            continue;
        }
        else if (c == 'q')
            break;
        else
            continue;
        game->num_turns += 1;
        addRandomSquare(game);
	}
	clrtoeol();
    clear();
    printw("Final game position\n");
    printGame(game);
	refresh();
	endwin();
    free2dArr(game->board, game->num_rows);
    free(game->board);
}