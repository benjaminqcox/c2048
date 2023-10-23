#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "game.h"

#define INSERT_USER_MAX_LENGTH 512
// Need to calculate the actual value of this
#define INSERT_USERS_GAME_MAX_LENGTH 512 
// Need to calculate the actual value of this
#define INSERT_GAME_STATE_MAX_LENGTH 512 

void connectToDatabase(MYSQL *conn, char *server, char *user, char *password, char *database)
{
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void createDatabase(MYSQL *conn)
{
    const char *createDatabase = "CREATE DATABASE IF NOT EXISTS c2048;";
    if (mysql_query(conn, createDatabase)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void createUsersTable(MYSQL *conn)
{
    const char *createUserTable = 
    "CREATE TABLE IF NOT EXISTS "
    "users("
        "user_id int NOT NULL AUTO_INCREMENT, "
        "username nvarchar(60) UNIQUE NOT NULL, "
        "email nvarchar(320) UNIQUE NOT NULL, "
        "password varchar(64) NOT NULL, "
        "PRIMARY KEY (user_id)"
    ");";
    if (mysql_query(conn, createUserTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void createUsersGamesTable(MYSQL *conn)
{
    const char *createUsersGamesTable = 
    "CREATE TABLE IF NOT EXISTS "
    "usersGames("
        "game_id int NOT NULL AUTO_INCREMENT, "
        "user_id int NOT NULL, "
        "num_rows int NOT NULL, "
        "num_cols int NOT NULL, "
        "PRIMARY KEY (game_id), "
        "FOREIGN KEY (user_id) REFERENCES users(user_id) "
    ");";
    if (mysql_query(conn, createUsersGamesTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void createGameStatesTable(MYSQL *conn)
{
    const char *createGameStatesTable = 
    "CREATE TABLE IF NOT EXISTS "
    "gameStates("
        "game_id int NOT NULL, "
        "score int NOT NULL, "
        "turns int NOT NULL, "
        "state nvarchar(100) NOT NULL, "
        "moves_in_past int NOT NULL"
    ");";

    if (mysql_query(conn, createGameStatesTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void insertUser(MYSQL *conn, char *username, char *email, char *password)
{
    char insertUserCom[INSERT_USER_MAX_LENGTH];
    snprintf(insertUserCom, INSERT_USER_MAX_LENGTH, "INSERT INTO users(username, email, password) VALUES(\"%s\", \"%s\", \"%s\");", username, email, password);
    if (mysql_query(conn, insertUserCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void insertUsersGame(MYSQL *conn, int userId, int numColumns, int numRows)
{
    //UNTESTED
    // This will be executed everytime the user presses new game
    char insertUsersGameCom[INSERT_USERS_GAME_MAX_LENGTH];
    snprintf(insertUsersGameCom, INSERT_USERS_GAME_MAX_LENGTH, "INSERT INTO usersGames(user_id, num_rows, num_cols) VALUES(%d, %d, %d);", userId, numRows, numColumns);
    if (mysql_query(conn, insertUsersGameCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void insertGameState(MYSQL *conn, int gameId, int score, int turns, char *gameState, int movesInPast)
{
    //UNTESTED
    char insertGameStateCom[INSERT_USER_MAX_LENGTH];
    snprintf(insertGameStateCom, INSERT_USER_MAX_LENGTH, "INSERT INTO gameStates(game_id, score, turns, state, moves_in_past) VALUES(%d, %d, %d, \"%s\", %d);", gameId, score, turns, gameState, movesInPast);
    if (mysql_query(conn, insertGameStateCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void useDatabase(MYSQL *conn)
{
    const char *useDB = "use c2048;";
    if (mysql_query(conn, useDB)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void showTables(MYSQL *conn)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    const char *tables = "show tables;";
    if (mysql_query(conn, tables)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_use_result(conn);
    printf("MySQL Tables in current database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);
    mysql_free_result(res);
}

void printTable(MYSQL *conn, char *table)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    char columnsCom[64];
    unsigned int numColumns;
    snprintf(columnsCom, 64, "DESCRIBE `%s`;", table);
    if (mysql_query(conn, columnsCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_store_result(conn);
    numColumns = mysql_num_rows(res);

    char selectTableCom[64];
    snprintf(selectTableCom, 64, "SELECT * FROM `%s`;", table);
    if (mysql_query(conn, selectTableCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_use_result(conn);
    printf("Rows in %s table:\n", table);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        for (int i = 0; i < numColumns; i++)
        {
            printf("%s", row[i]);
            if (i < numColumns -1)
            {
                printf(", ");
            }
        }
        printf("\n");
    }

    mysql_free_result(res);
}

int selectUserId(MYSQL *conn, char* username)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    char selectUserIdCom[64];
    snprintf(selectUserIdCom, 64, "SELECT `user_id` FROM `users` WHERE `username` LIKE '%%%s%%';", username);
    if (mysql_query(conn, selectUserIdCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL)
    {
        fprintf(stderr, "User not found.\n");
        return -1;
    }
    return atoi(row[0]); // do not keep as atoi
}

MYSQL_RES *selectUsersGame(MYSQL *conn, int userId)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    char selectUsersGameCom[64];
    snprintf(selectUsersGameCom, 64, "SELECT * FROM `usersGames` WHERE `user_id`=%d;", userId);
    if (mysql_query(conn, selectUsersGameCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_store_result(conn);
    return res;
}

MYSQL_RES *selectGameStates(MYSQL *conn, int gameId)
{
    MYSQL_RES *res = NULL;
    char selectGameStatesCom[64];
    snprintf(selectGameStatesCom, 64, "SELECT * FROM `gameStates` WHERE `game_id`=%d;", gameId);
    if (mysql_query(conn, selectGameStatesCom)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_store_result(conn);
    return res;
}