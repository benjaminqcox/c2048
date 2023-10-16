#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ncurses.h>
#include "shared.h"
#include "userInput.h"
#include "game.h"


// Quit program and close socket
void quitProgram(int sock)
{
    // Send Quit option to server to indicate closing connection
    int option = QUIT;
    if (send(sock, &option, sizeof(option), 0) < 0) {
        perror("Send error");
        return;
    }
    // close the socket
    close(sock);
    // inform user that client has disconnected from server
    printw("Disconnected from the server. Goodbye!\n");
    refresh();
}

void newGame(int sock)
{
    int option = NEW_GAME;
    if (send(sock, &option, sizeof(option), 0) < 0) {
        perror("Send error");
        return;
    }
    // check game was created
}

game_t getGameState(int sock)
{
    game_t *currentGame = NULL;
    if (recv(sock, &currentGame, sizeof(currentGame), 0) < 0) // Get current game state
    {
        perror("Receive error");
        exit(EXIT_FAILURE);
    }
    return *currentGame;
}

void chooseMove(int sock)
{
    game_t currentGame;
    if (recv(sock, &currentGame, sizeof(currentGame), 0) < 0)
    {
        perror("Receive error");
        exit(EXIT_FAILURE);
    }
    printw("here\n;");
    int c;
    clear();
    printw("Use wasd or arrow keys to choose move, r to reset and q to quit\n");
    printGame(&currentGame);
    refresh();
    c = getch();
    // Check if valid move here first?
    if (send(sock, &c, sizeof(c), 0), 0)
    {
        perror("Send error");
        exit(EXIT_FAILURE);
    }
}

bool isGameOver(int sock)
{
    bool isGameOver = false;
    if (recv(sock, &isGameOver, sizeof(isGameOver), 0) < 0) // Get current game state
    {
        perror("Receive error");
        exit(EXIT_FAILURE);
    }
    return isGameOver;
}

int recv_int(int socket) {
    int value;
    recv(socket, &value, sizeof(value), 0);
    printw("value recieved = %d\n", value);
    return value;
}

int main(int argc, char const *argv[])
{
    // create a socket
    int sock, connection_result, n;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[64];

    // create a tcp socket
    sock = socket(AF_INET, SOCK_STREAM, TCP_MODE);
    if (sock < 0) {
        perror("[-]Socket error\n");
        exit(1);
    }
    printf("[+] TCP server socket created.\n");

    // Assign IP, Port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // Address family INET = Internet
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, CONNECTION_ADDRESS, &addr.sin_addr);

    // Connect to server
    connection_result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (connection_result == -1) {
        perror("[-] Connection error");
        exit(1);
    }
    printf("Connected to server.\n");

    // set up variable for menu option
    int menuOption;
    bool game_started = false;
    int c;
    game_t *current_game;

    initscr();
    clear();

    while (true) {
        // first check if game has started by receiving information from server
        if (recv(sock, &game_started, sizeof(game_started), 0) < 0) // Get current game state
        {
            perror("Receive error");
            exit(EXIT_FAILURE);
        }
        printw("game started = %d\n", game_started);
        refresh();
        if (game_started) // if it has happened, we want to show a different set of menu options
        {
            // ENDED HERE TO FIX THIS WHILE LOOP
            // GAME IS NOT CURRENTLY DISPLAYING THE GAME BOARD
            refresh();
            int num_columns = recv_int(sock);
            int num_rows = recv_int(sock);
            current_game = makeGame(num_columns, num_rows);

            
            while (true)  // (!isGameOver(sock))
            {
                // Clear all screen output and request from the server: num turns, score, entire board
                clear();
                recv(sock, &(current_game->num_turns), sizeof(int), 0);
                recv(sock, &(current_game->score), sizeof(int), 0);
                for (int i = 0; i < (current_game->num_columns * current_game->num_rows); i++)
                        recv(sock, &(current_game->board[i]), sizeof(int), 0);
                
                // Display user options and print game
                printw("Use wasd or arrow keys to choose move, r to reset and q to quit\n");
                printGame(current_game);
                refresh();
                // Get user input
                c = getch();
                // Could loop until valid choice here but the server already deals with the input
                if (send(sock, &c, sizeof(c), 0), 0)
                {
                    perror("Send error");
                    exit(EXIT_FAILURE);
                }
                if (c == 'q')
                {
                    // just to go back te menu
                    game_started = false;
                    break;
                }
            }
        }
        else
        {
            // Reset screen output and display menu options to user
            clear();
            printw("Select your option:\n[0] Disconnect from server\n[1] Start new game\n[2] Load game\n");
            refresh();
            // Get user input
            c = getch();
            refresh();
            // switch statement for all options
            switch (c) {
                case QUIT:
                    // close the socket and disconnect from server, and exit program
                    quitProgram(sock);
                    exit(EXIT_SUCCESS);
                    break;
                case NEW_GAME:
                    newGame(sock);
                    break;
                case LOAD_GAME:
                    printw("Loading previous games...\n");
                    printw("no implementation added here yet\n");
                    refresh();
                    break;
                default:
                    //invalid input (could let user know but there is a clear statement on loopback 
                    // so anything printed will likely not be seen)
                    break;
            }
        }
    }
    // if exiting the infinite loop, close the socket first
    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}