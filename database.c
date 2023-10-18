#include "/opt/homebrew/opt/mysql/include/mysql/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSERT_USER_MAX_LENGTH 512

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
        "userId int NOT NULL AUTO_INCREMENT, "
        "username nvarchar(60) UNIQUE NOT NULL, "
        "email nvarchar(320) UNIQUE NOT NULL, "
        "password varchar(64) NOT NULL, "
        "PRIMARY KEY (userId)"
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
        "gameId int NOT NULL AUTO_INCREMENT, "
        "userId int NOT NULL, "
        "numrows int NOT NULL, "
        "numcols int NOT NULL, "
        "PRIMARY KEY (gameId), "
        "FOREIGN KEY (userId) REFERENCES users(userId) "
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
        "gameId int NOT NULL, "
        "score int NOT NULL, "
        "turns int NOT NULL, "
        "state nvarchar(100) NOT NULL"
    ");";

    if (mysql_query(conn, createGameStatesTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void insertUser(MYSQL *conn, char *username, char *email, char *password)
{
    char insertUser[INSERT_USER_MAX_LENGTH];
    snprintf(insertUser, INSERT_USER_MAX_LENGTH, "INSERT INTO users(username, email, password) VALUES(\"%s\", \"%s\", \"%s\");", username, email, password);
    if (mysql_query(conn, insertUser)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
}

void insertUsersGame(MYSQL *conn, char *username, char *email, char *password)
{
    // char insertUser[INSERT_USER_MAX_LENGTH];
    // snprintf(insertUser, INSERT_USER_MAX_LENGTH, "INSERT INTO users(username, email, password) VALUES(\"%s\", \"%s\", \"%s\");", username, email, password);
    // if (mysql_query(conn, insertUser)) {
    //     fprintf(stderr, "%s\n", mysql_error(conn));
    //     exit(EXIT_FAILURE);
    // }
}

void insertGameState(MYSQL *conn, char *username, char *email, char *password)
{
    // char insertUser[INSERT_USER_MAX_LENGTH];
    // snprintf(insertUser, INSERT_USER_MAX_LENGTH, "INSERT INTO users(username, email, password) VALUES(\"%s\", \"%s\", \"%s\");", username, email, password);
    // if (mysql_query(conn, insertUser)) {
    //     fprintf(stderr, "%s\n", mysql_error(conn));
    //     exit(EXIT_FAILURE);
    // }
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

void printUsersTable(MYSQL *conn)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    const char *tables = "select * from users;";
    if (mysql_query(conn, tables)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    res = mysql_use_result(conn);
    printf("Rows in users table:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s, %s, %s, %s \n", row[0], row[1], row[2], row[3]);
    mysql_free_result(res);
}

int main() {
    
    MYSQL *conn = mysql_init(NULL);
    connectToDatabase(conn, "localhost", "root", "", "mysql");
    createDatabase(conn);
    useDatabase(conn);
    createUsersTable(conn);
    createUsersGamesTable(conn);
    createGameStatesTable(conn);
    insertUser(conn, "benkei", "themonsterzclan@gmail.com", "plaintext");
    showTables(conn);
    printUsersTable(conn);

    mysql_close(conn);
}

