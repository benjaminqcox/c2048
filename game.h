#define MAX_ROWS 4
#define MAX_COLUMNS 4

typedef struct game
{
    int **board;
    int score;
    int num_turns;
    int num_rows;
    int num_columns;
} game_t;

void printGame(game_t *game);

void printGame2(game_t game);

void free2dArr(int **arr, int numrows);

void freeGame(game_t *game);

void print2dArr(game_t *game);

int **create2dArr(int numrows, int numcolumns);

void reset2dArr(int **arr, int num_rows, int num_columns);

void resetGame(game_t *game);

game_t *makeGame(int num_rows, int num_columns);

void squashLeft(game_t *game);

void squashRight(game_t *game);

void squashUp(game_t *game);

void squashDown(game_t *game);

int countEmpty(game_t *game);

void addRandomSquare(game_t *game);

void playGame(game_t *game);