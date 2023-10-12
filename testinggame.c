#include <stdio.h>
#include <stdlib.h>


// This is an iteration using 1d arrays for ease of db storage and sending through a server client connection

int getPos(int row, int col, int rows, int cols)
{
    return row * cols + col;
}

void printBox(int *game, int rows, int cols)
{
    int pos;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            pos = getPos(row, col, rows, cols);
            printf("%d ", game[pos]);
        }
        printf("\n");
    }
}



void update(int *game, int val, int row, int col, int rows, int cols)
{
    int pos = getPos(row, col, rows, cols);
    printf("%d\n", pos);
    game[pos] = val;
}

void squashRight(int *game, int rows, int cols)
{
    // This function has too many indents, need to separate it to make it cleaner and easier to read

    // Loop through all rows
    // Loop through all columns x
    // Loop through all columns from the current column y
    // if the y == 0 or y == x then combine the column values
    int col, old, new;
    for (int row = 0; row < rows; row++)
    {
        col = cols-1;
        while (col > 0)
        {
            for (int comb = col-1; comb >= 0; comb--)
            {
                old = getPos(row, comb, rows, cols);
                if (game[old] > 0)
                {
                    new = getPos(row, col, rows, cols);
                    if (game[new] == 0)
                    {
                        game[new] = game[old];
                        game[old] = 0;
                        col++;
                    }
                    else if(game[new] == game[old])
                    {
                        game[new] += game[old];
                        //score += game[new];
                        game[old] = 0;
                    }
                    break;
                }
            }
            col--;
        }
    }
}

int main()
{
    int rows = 4;
    int cols = 4;
    int *box = NULL;
    box = (int *)malloc(rows*cols*sizeof(int));
    for (int i = 0; i < rows*cols; i++)
    {
        box[i] = 0;
    }
    update(box, 2, 2, 2, rows, cols);
    update(box, 2, 2, 1, rows, cols);
    squashRight(box, rows, cols);

    printBox(box, rows, cols);

    free(box);
    box = NULL;
    return 0;
}