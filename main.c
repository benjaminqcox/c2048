#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "userInput.h"

#define MAX_ROWS 4
#define MAX_COLUMNS 4


void free2dArr(int **arr, int numrows)
{
    for (int row = 0; row < numrows; row++)
    {
        free(arr[row]);
        arr[row] = NULL;
    }
}

void print2dArr(int **arr, int numrows, int numcolumns)
{
    printf("##########\n");
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcolumns; col++)
        {
            if (col == numcolumns-1)
            {
                printf("%d\n", arr[row][col]);
            }
            else
            {
                printf("%d, ", arr[row][col]);
            }   
        }
    }
    printf("##########\n");
}

int **create2dArr(int numrows, int numcolumns)
{
    // Allocate memory for 2d array
    int **new2dArr = (int**)malloc(numrows * sizeof(int*));
    // Check memory was allocated correctly for the rows
    if (new2dArr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for rows.");
        exit(EXIT_FAILURE);
    }
    
    for (int row = 0; row < numrows; row++)
    {
        // Allocate memory for each column in the array
        new2dArr[row] = (int *)malloc(numcolumns * sizeof(int));
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

void reset2dArr(int **arr, int numrows, int numcolumns)
{
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcolumns; col++)
        {
            arr[row][col] = 0;
        }
    }
}


void combineLeft()
{
    // this function can be used to take all values greater than 0 and combine them
}

void shiftLeft()
{
    // this function can be used to take all values in a row and shift them to the left as far as possible
}

void squashLeft(int **arr, int numrows, int numcolumns)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < numrows; row++)
    {
        col = 0;
        while (col < numcolumns)
        {
            for (int comb = col+1; comb < numcolumns; comb++)
            {
                if (arr[row][comb] > 0)
                {
                    if (arr[row][col] == 0)
                    {
                        arr[row][col] = arr[row][comb];
                        arr[row][comb] = 0;
                        col--;
                    }
                    else if(arr[row][col] == arr[row][comb])
                    {
                        arr[row][col] += arr[row][comb];
                        arr[row][comb] = 0;
                    }
                    break;
                }
            }
            col++;
        }
    }
}


void squashRight(int **arr, int numrows, int numcolumns)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = numrows-1; row >= 0; row--)
    {
        col = numcolumns-1;
        while (col > 0)
        {
            for (int comb = col-1; comb >= 0; comb--)
            {
                if (arr[row][comb] > 0)
                {
                    if (arr[row][col] == 0)
                    {
                        arr[row][col] = arr[row][comb];
                        arr[row][comb] = 0;
                        col++;
                    }
                    else if(arr[row][col] == arr[row][comb])
                    {
                        arr[row][col] += arr[row][comb];
                        arr[row][comb] = 0;
                    }
                    break;
                }
            }
            col--;
        }
    }
}


void squashUp(int **arr, int numrows, int numcolumns)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < numrows; row++)
    {
        col = 0;
        while (col < numcolumns)
        {
            for (int comb = col+1; comb < numcolumns; comb++)
            {
                if (arr[comb][row] > 0)
                {
                    if (arr[col][row] == 0)
                    {
                        arr[col][row] = arr[comb][row];
                        arr[comb][row] = 0;
                        col--;
                    }
                    else if(arr[col][row] == arr[comb][row])
                    {
                        arr[col][row] += arr[comb][row];
                        arr[comb][row] = 0;
                    }
                    break;
                }
            }
            col++;
        }
    }
}

void squashDown(int **arr, int numrows, int numcolumns)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col;
    for (int row = 0; row < numrows; row++)
    {
        col = numcolumns-1;
        while (col > 0)
        {
            for (int comb = col-1; comb >= 0; comb--)
            {
                if (arr[comb][row] > 0)
                {
                    if (arr[col][row] == 0)
                    {
                        arr[col][row] = arr[comb][row];
                        arr[comb][row] = 0;
                        col++;
                    }
                    else if(arr[col][row] == arr[comb][row])
                    {
                        arr[col][row] += arr[comb][row];
                        arr[comb][row] = 0;
                    }
                    break;
                }
            }
            col--;
        }
    }
}

int countEmpty(int **arr, int numrows, int numcolumns)
{
    int numEmpty = 0;
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcolumns; col++)
        {
            if (arr[row][col] ==  0)
            {
                numEmpty++;
            }
        }
    }
    return numEmpty;
}

void addRandomSquare(int **arr, int numrows, int numcolumns)
{
    int numEmpty = countEmpty(arr, numrows, numcolumns);
    // Check if numEmpty == 0 meaning there are no spare squares available for use
    int r = rand() % numEmpty + 1;
    int emptyCount = 0;
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcolumns; col++)
        {
            if (arr[row][col] == 0 && ++emptyCount == r)
            {
                // Generate a random number between 1 and 100
                // if it is greater than 10, then the value is 2
                // otherwise the value is 4
                // 10% chance of being a 4 value
                arr[row][col] = ((rand() + 1) % 100) > 10 ? 2 : 4;
            }
        }
    }
}



// need function to add 2 or 4 (at a 10% rate) to a random emptcol (val 0) position

// need functions to 'squash' the 2d array for every move (up, down, left, right)

// need a quit function (cleanup function)

int main()
{
    // Need to add scoring
    // need to add a gameover method
    srand((unsigned int)time(NULL));
    int **gameBoard = create2dArr(MAX_ROWS, MAX_COLUMNS);
    reset2dArr(gameBoard, MAX_ROWS, MAX_COLUMNS);
    bool quit = false;
    int choice;
    while (!quit)
    {
        system("clear");
        addRandomSquare(gameBoard, MAX_ROWS, MAX_COLUMNS);
        print2dArr(gameBoard, MAX_ROWS, MAX_COLUMNS);
        printf("1 - left, 2 - right, 3 - up, 4 - down, 5 - reset, 0 - quit\n");
        choice = getIntInputInRange(0, 5);
        switch (choice)
        {
            case 1:
                squashLeft(gameBoard, MAX_ROWS, MAX_COLUMNS);
                break;
            case 2:
                squashRight(gameBoard, MAX_ROWS, MAX_COLUMNS);
                break;
            case 3:
                squashUp(gameBoard, MAX_ROWS, MAX_COLUMNS);
                break;
            case 4:
                squashDown(gameBoard, MAX_ROWS, MAX_COLUMNS);
                break;
            case 5:
                reset2dArr(gameBoard, MAX_ROWS, MAX_COLUMNS);
                break;
            case 0:
                printf("You have chosen to quit the game.");
                quit = true;
                break;
            default:
                printf("Unknown option. No changes have been made.\n");
                break;
        }
    }
    system("clear");
    printf("Final game positio:\n");
    print2dArr(gameBoard, MAX_ROWS, MAX_COLUMNS);
    free2dArr(gameBoard, MAX_ROWS);
    free(gameBoard);
    return 0;
}