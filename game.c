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
    // Convert 2d array position to 1d array position
    return game->num_columns * row + col;
}

int getVal(game_t *game, int col, int row)
{
    // Get value stored at a 2d array position
    return game->board[getPos(game, col, row)];
}

void updatePos(game_t *game, int val, int row, int col)
{
    // Update a value stored at a 2d array position
    int pos = getPos(game, col, row);
    game->board[pos] = val;
}

void printGame(game_t *game)
{
    // Print the game stats and board in the form of ncurses
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

void printBoard(int *board, int num_columns, int num_rows)
{
    for (int col = 0; col < num_columns; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            //printw("(%d, %d)", row, col);
            // %-* width adds 
            printf("%-*d", STRING_WIDTH, board[num_columns * row + col]);
        }
        printf("\n");
    }
}

int *createBoard(int num_columns, int num_rows)
{
    // Generate new game board array with a dynamically chosen size
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
    // Set all values on the board to 0 (initial state)
    int board_size = num_columns * num_rows;
    for (int i = 0; i < board_size; i++)
    {
        arr[i] = 0;
    }
}

void resetGame(game_t *game)
{
    // Reset all game values including the board
    resetBoard(game->board, game->num_columns, game->num_rows);
    game->score = 0;
    game->num_turns = 0;
}

game_t *makeGame(int num_columns, int num_rows)
{
    // Generate all game values including the board
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
    // Given a 2d array force all values to the left as much as they can
    // e.g. 0 2 0 2 -> 4 0 0 0
    //      0 2 0 4 -> 2 4 0 0
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
    // Given a 2d array force all values to the right as much as they can
    // e.g. 0 2 0 2 -> 0 0 0 4
    //      0 2 0 4 -> 0 0 2 4
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
    // Given a 2d array force all values to the up as much as they can
    // e.g. 0 2 0 2 -> 2 4 0 2
    //      2 2 0 4 -> 0 0 0 4
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
    // Given a 2d array force all values to the up as much as they can
    // e.g. 2 2 0 2 -> 0 0 0 2
    //      0 2 0 4 -> 2 4 0 4
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
    // Find the number of board spaces that contain the value 0
    int numEmpty = 0;
    for (int i = 0; i < (game->num_columns * game->num_rows); i++)
        if (game->board[i] ==  0)
            numEmpty++;
    return numEmpty;
}

void addRandomSquare(game_t *game)
{
    // Get the number of empty spaces and generate a random number between 1 and that number
    // Go through the empty spaces until that number has been reached 
    //  and set that value to 2 or a 10% chance of being 4
    int numEmpty = countEmpty(game);
    int r = rand() % numEmpty + 1;
    int emptyCount = 0;
    for (int i = 0; i < (game->num_columns * game->num_rows); i++)
    {
        if (game->board[i] == 0 && ++emptyCount == r)
        {
            game->board[i] = ((rand() + 1) % 100) > 10 ? 2 : 4;
            break;
        }
    }
}

char *boardToString(int *board, int num_columns, int num_rows)
{
    int boardSize = num_columns * num_rows;
    char *strBoard = (char *)malloc(boardSize);
    if (strBoard == NULL)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < boardSize; i++)
    {
        strBoard[i] = board[i] + '0';
    }
    strBoard[boardSize] = '\0';
    return strBoard;
}

int *stringToBoard(char *strBoard, int num_columns, int num_rows)
{
    int boardSize = num_columns * num_rows;
    int *board = (int *)malloc(boardSize);
    if (board == NULL)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < boardSize; i++)
    {
        board[i] = strBoard[i] - '0';
    }
    return board;
}