#include "/opt/homebrew/opt/mysql/include/mysql/mysql.h"

#define INSERT_USER_MAX_LENGTH 512
// Need to calculate the actual value of this
#define INSERT_USERS_GAME_MAX_LENGTH 512 
// Need to calculate the actual value of this
#define INSERT_GAME_STATE_MAX_LENGTH 512 

void connectToDatabase(MYSQL *conn, char *server, char *user, char *password, char *database);

void createDatabase(MYSQL *conn);

void createUsersTable(MYSQL *conn);

void createUsersGamesTable(MYSQL *conn);

void createGameStatesTable(MYSQL *conn);

void insertUser(MYSQL *conn, char *username, char *email, char *password);

void insertUsersGame(MYSQL *conn, int userId, int numRows, int numColumns);

void insertGameState(MYSQL *conn, int gameId, int score, int turns, char *gameState, int movesInPast);

void useDatabase(MYSQL *conn);

void showTables(MYSQL *conn);

void printTable(MYSQL *conn, char *table);

int selectUserId(MYSQL *conn, char* username);

MYSQL_RES *selectUsersGame(MYSQL *conn, int userId);

MYSQL_RES *selectGameStates(MYSQL *conn, int gameId);

