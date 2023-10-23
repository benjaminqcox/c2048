#define MAX_ROWS 4
#define MAX_COLUMNS 4

typedef struct game
{
    int *board;
    int score;
    int num_turns;
    int num_rows;
    int num_columns;
} game_t;

int getPos(game_t *game, int col, int row);

int getVal(game_t *game, int col, int row);

void updatePos(game_t *game, int val, int row, int col);

void printGame(game_t *game);

void printBoard(int *board, int num_rows, int num_columns);

int *createBoard(int numrows, int numcolumns);

void resetBoard(int *arr, int num_rows, int num_columns);

void resetGame(game_t *game);

game_t *makeGame(int num_rows, int num_columns);

void squashLeft(game_t *game);

void squashRight(game_t *game);

void squashUp(game_t *game);

void squashDown(game_t *game);

int countEmpty(game_t *game);

void addRandomSquare(game_t *game);

char *boardToString(int *board, int num_columns, int num_rows);

int *stringToBoard(char *strBoard, int num_columns, int num_rows);