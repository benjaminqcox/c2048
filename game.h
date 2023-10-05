#define MAX_ROWS 4
#define MAX_COLUMNS 4


void free2dArr(int **arr, int numrows);

void print2dArr(int **arr, int numrows, int numcolumns);

int **create2dArr(int numrows, int numcolumns);

void reset2dArr(int **arr, int numrows, int numcolumns);

void squashLeft(int **arr, int numrows, int numcolumns);

void squashRight(int **arr, int numrows, int numcolumns);

void squashUp(int **arr, int numrows, int numcolumns);

void squashDown(int **arr, int numrows, int numcolumns);

int countEmpty(int **arr, int numrows, int numcolumns);

void addRandomSquare(int **arr, int numrows, int numcolumns);

void playGame();