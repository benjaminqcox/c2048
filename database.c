#include "/opt/homebrew/opt/mysql/include/mysql/mysql.h"
#include <stdio.h>
#include <stdlib.h>
 
int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "localhost";
    char *user = "root";
    char *password = "";
    char *database = "mysql";
    conn = mysql_init(NULL);
    /* Connect to database */
    if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
    }

    const char *createdatabase = "CREATE DATABASE IF NOT EXISTS c2048;";
    const char *createUserTable = "CREATE TABLE IF NOT EXISTS users(userId int, username nvarchar(60), email nvarchar(320), password varchar(64));";
    const char *createGameStatesTable = "CREATE TABLE IF NOT EXISTS gameStates(gameId int, score int, turns int, state nvarchar(100));";
    const char *createUserGamesTable = "CREATE TABLE IF NOT EXISTS userGames(userId int, gameId int, numrows int, numcols int);";


    if (mysql_query(conn, createdatabase)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    /* send SQL query */
    if (mysql_query(conn, "use c2048;")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }


    if (mysql_query(conn, createUserTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, createGameStatesTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, createUserGamesTable)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, "show tables;")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn);

    /* output table name */
    printf("MySQL Tables in c2048 database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);

    /* close connection */
    mysql_free_result(res);
    mysql_close(conn);
}

