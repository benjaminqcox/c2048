#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include "userInput.h"
#include "game.h"

#define STRING_WIDTH 5

int getPos(game_t *game, int col, int row)
{
    return game->num_columns * row + col;
}

int getVal(game_t *game, int col, int row)
{
    return game->board[getPos(game, col, row)];
}

void updatePos(game_t *game, int val, int row, int col)
{
    int pos = getPos(game, col, row);
    game->board[pos] = val;
}

void printGame(game_t *game)
{
    printw("Score: %d, Turns: %d\n", game->score, game->num_turns);
    for (int col = 0; col < game->num_columns; col++)
    {
        for (int row = 0; row < game->num_rows; row++)
        {
            //printw("(%d, %d)", row, col);
            // %-* width adds 
            printw("%-*d", STRING_WIDTH, getVal(game, col, row));
        }
        printw("\n");
    }
}

void printGame2(game_t *game)
{
    for (int col = 0; col < game->num_columns; col++)
    {
        for (int row = 0; row < game->num_rows; row++)
        {
            printf("%d ", getVal(game, col, row));
        }
        printf("\n");
    }
}

int *createBoard(int num_columns, int num_rows)
{
    int board_size = num_columns * num_rows;
    // Allocate memory for the game board
    int *board = (int*)malloc(board_size * sizeof(int));
    // Check memory was allocated correctly for the rows
    if (board == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for rows.");
        exit(EXIT_FAILURE);
    }
    
    resetBoard(board, num_columns, num_rows);
    return board;
}

void resetBoard(int *arr, int num_columns, int num_rows)
{
    int board_size = num_columns * num_rows;
    for (int i = 0; i < board_size; i++)
    {
        arr[i] = 0;
    }
}

void resetGame(game_t *game)
{
    resetBoard(game->board, game->num_columns, game->num_rows);
    game->score = 0;
    game->num_turns = 0;
}

game_t *makeGame(int num_columns, int num_rows)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    if (game == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for game.\n");
        exit(EXIT_FAILURE);
    }
    game->board = createBoard(num_rows, num_columns);
    game->num_rows = num_rows;
    game->num_columns = num_columns;
    resetGame(game);
    return game;
}

void squashLeft(game_t *game)
{
    int finalIndex, current, final;
    for (int row = 0; row < game->num_rows; row++)
    {
        finalIndex = 0;
        while (finalIndex < game->num_columns)
        {
            for (int col = finalIndex+1; col < game->num_columns; col++)
            {
                current = getPos(game, row, col);
                if (game->board[current] > 0)
                {
                    final = getPos(game, row, finalIndex);
                    if (game->board[final] == 0)
                    {
                        game->board[final] = game->board[current];
                        game->board[current] = 0;
                        finalIndex--;
                    }
                    else if(game->board[final] == game->board[current])
                    {
                        game->board[final] += game->board[current];
                        game->board[current] = 0;
                        game->score += game->board[final];
                    }
                    break;
                }
            }
            finalIndex++;
        }
    }
}

void squashRight(game_t *game)
{
    int finalIndex, current, final;
    for (int row = 0; row < game->num_rows; row++)
    {
        finalIndex = (game->num_columns)-1;
        while (finalIndex > 0)
        {
            for (int col = finalIndex-1; col >= 0; col--)
            {
                current = getPos(game, row, col);
                if (game->board[current] > 0)
                {
                    final = getPos(game, row, finalIndex);
                    if (game->board[final] == 0)
                    {
                        game->board[final] = game->board[current];
                        game->board[current] = 0;
                        finalIndex++;
                    }
                    else if(game->board[final] == game->board[current])
                    {
                        game->board[final] += game->board[current];
                        game->board[current] = 0;
                        game->score += game->board[final];
                    }
                    break;
                }
            }
            finalIndex--;
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
    int finalIndex, current, final;
    for (int row = 0; row < game->num_rows; row++)
    {
        finalIndex = 0;
        while (finalIndex < game->num_columns)
        {
            for (int col = finalIndex+1; col < game->num_columns; col++)
            {
                current = getPos(game, col, row);
                if (game->board[current] > 0)
                {
                    final = getPos(game, finalIndex, row);
                    if (game->board[final] == 0)
                    {
                        game->board[final] = game->board[current];
                        game->board[current] = 0;
                        finalIndex--;
                    }
                    else if(game->board[final] == game->board[current])
                    {
                        game->board[final] += game->board[current];
                        game->board[current] = 0;
                        game->score += game->board[final];
                    }
                    break;
                }
            }
            finalIndex++;
        }
    }
}

void squashDown(game_t *game)
{
    int finalIndex, current, final;
    for (int row = 0; row < game->num_rows; row++)
    {
        finalIndex = (game->num_columns)-1;
        while (finalIndex > 0)
        {
            for (int col = finalIndex-1; col >= 0; col--)
            {
                current = getPos(game, col, row);
                if (game->board[current] > 0)
                {
                    final = getPos(game, finalIndex, row);
                    if (game->board[final] == 0)
                    {
                        game->board[final] = game->board[current];
                        game->board[current] = 0;
                        finalIndex++;
                    }
                    else if(game->board[final] == game->board[current])
                    {
                        game->board[final] += game->board[current];
                        game->board[current] = 0;
                        game->score += game->board[final];
                    }
                    break;
                }
            }
            finalIndex--;
        }
    }
}

int countEmpty(game_t *game)
{
    int numEmpty = 0;
    for (int i = 0; i < (game->num_columns * game->num_rows); i++)
        if (game->board[i] ==  0)
            numEmpty++;
    return numEmpty;
}

void addRandomSquare(game_t *game)
{
    int numEmpty = countEmpty(game);
    // Check if numEmpty == 0 meaning there are no spare squares available for use
    int r = rand() % numEmpty + 1;
    int emptyCount = 0;
    for (int i = 0; i < (game->num_columns * game->num_rows); i++)
    {
        if (game->board[i] == 0 && ++emptyCount == r)
        {
            // Generate a random number between 1 and 100
            // if it is greater than 10, then the value is 2
            // otherwise the value is 4
            // 10% chance of being a 4 value
            game->board[i] = ((rand() + 1) % 100) > 10 ? 2 : 4;
            break;
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
    free(game->board);
}