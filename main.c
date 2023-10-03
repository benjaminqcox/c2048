#include <stdio.h>
#include <stdlib.h>

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

// need function to add 2 or 4 (at a 10% rate) to a random emptcol (val 0) position

// need functions to 'squash' the 2d array for every move (up, down, left, right)

// need a quit function (cleanup function)

int main()
{
    
    int **test2dArr = create2dArr(MAX_ROWS, MAX_COLUMNS);
    reset2dArr(test2dArr, MAX_ROWS, MAX_COLUMNS);
    test2dArr[1][1] = 5;
    print2dArr(test2dArr, MAX_ROWS, MAX_COLUMNS);
    reset2dArr(test2dArr, MAX_ROWS, MAX_COLUMNS);
    print2dArr(test2dArr, MAX_ROWS, MAX_COLUMNS);
    free2dArr(test2dArr, MAX_ROWS);
    free(test2dArr);
    return 0;
}