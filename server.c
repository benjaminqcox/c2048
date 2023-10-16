#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include "shared.h"
#include "game.h"

/*
// Struct format
struct sockaddr_in {
    // sin = sockaddr_in -> s_in -> sin
    sa_family_t sin_family; // address family: AF_INET
    in_port_t sin_port; // port in network byte order
    struct in_addr sin_addr; // internet address
};

// Internet address
struct in_addr {
    uint32_t s_addr; // address in network byte order
};
*/


void sendBoard(int sock, game_t game)
{
    for (int i = 0; i < (game.num_columns * game.num_rows); i++)
    {
        if (send(sock, &(game.board[i]), sizeof(game.board[i]), 0) < 0) // Get current game state
        {
            perror("Send error");
            exit(EXIT_FAILURE);
        }
    }
}



int main()
{
    // Declare the variables needed to store connection based information
    int ret, sd, cl_sd;
    struct sockaddr_in sv_addr, cl_addr;
    socklen_t addrlen = sizeof(cl_addr);
    ssize_t sent_bytes;
    bool active_client = false;



    // game related variables
    game_t *current_game = NULL;
    bool game_started = false;
    int menuChoice;
    srand((unsigned int)time(NULL));
    int c;

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, TCP_MODE);
    // Check successful socket creation
    if (sd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&sv_addr, 0, sizeof(sv_addr)); // sets the first count bytes of dest to the value c
    sv_addr.sin_family = AF_INET; // set the sockadress transport address to AF_INET (Address Family InterNET)
    sv_addr.sin_port = htons(SERVER_PORT); // converts unsigned short integer (hostshort) from host byte order to netword byte order
    inet_pton(AF_INET, CONNECTION_ADDRESS, &sv_addr.sin_addr); // converts IPv4 and IPv6 addresses from text to binary form

    // assigns the local socket address to a socket identified by descriptor socket that has no local socket address assigned
    // sockets created with socket() are initially unnamed; they are identified by their address family
    ret = bind(sd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)); 
    ret = listen(sd, BACKLOG_SIZE); // listen on the created socket with a maximum backlog size of 10
    // Check the listen was successfully setup
    if (ret == -1) {
        // If failed, exit the program
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Print port listening on
    printf("Server is listening at %s:%d...\n", CONNECTION_ADDRESS, SERVER_PORT);

    while (1)
    {
        // extract the first connection request in the queue of pending connections, create a new socket with the same socket type protocol
        // and address family as the specified socket, and allocate a new file descriptor for that socket
        cl_sd = accept(sd, (struct sockaddr*)&cl_addr, &addrlen);
        // Check client connection was made successfully
        if (cl_sd == -1) {
            // If not successful, loop back to wait for new connection
            perror("accept");
            continue;
        }
        
        if (fork() == 0)
        {
            // Close the connection to the main socket from the client and just use the client connection socket
            close(sd);
            printf("Server connected to client.\n");
            // The game has not started each time a child has been generated so set it to false
            game_started = false;
            
            // Loop the connection between the server and client for the menu and game
            while (true)
            {
                // Send to client the result of has_drawn
                if (send(cl_sd, &game_started, sizeof(game_started), 0) < 0) {
                    perror("Send\n");
                    exit(EXIT_FAILURE);
                }
                if (game_started)
                {
                    // Game needs a random value to start with
                    addRandomSquare(current_game);
                    // Constants that only need to be sent to the client once
                    send(cl_sd, &(current_game->num_rows), sizeof(int), 0);
                    send(cl_sd, &(current_game->num_columns), sizeof(int), 0);
                    
                    while(true)
                    {
                        // Send to the client every move: num turns, score and entire game board
                        send(cl_sd, &(current_game->num_turns), sizeof(current_game->num_turns), 0);
                        send(cl_sd, &(current_game->score), sizeof(current_game->score), 0);
                        sendBoard(cl_sd, *current_game);

                        // Receive the move selection from client
                        if (recv(cl_sd, &c, sizeof(c), 0))
                        {
                            perror("Receive error");
                            exit(EXIT_FAILURE);
                        }
                        // Perform action chosen by user
                        if (c == 'w' || c == 65)
                            squashUp(current_game);
                        else if (c == 'a' || c == 68)
                            squashLeft(current_game);
                        else if (c == 's' || c == 66)
                            squashDown(current_game);
                        else if (c == 'd' || c == 67)
                            squashRight(current_game);
                        else if (c == 'r')
                        {
                            resetGame(current_game);
                            addRandomSquare(current_game);
                            continue;
                        }
                        else if (c == 'q')
                        {
                            game_started = false;
                            break;
                        }
                        else
                            continue;
                        // Every time a move has been made, add num moves and add a random square to the board
                        current_game->num_turns += 1;
                        addRandomSquare(current_game);
                    }
                    // If the game is over, free the memory used
                    free(current_game->board);
                }
                else
                {
                    // Get the menuChoice from the client
                    if (recv(cl_sd, &menuChoice, sizeof(menuChoice), 0) < 0) {
                        perror("Receive\n");
                        exit(EXIT_FAILURE);
                    }
                    switch(menuChoice)
                    {
                        case NEW_GAME:
                            current_game = makeGame(MAX_ROWS, MAX_COLUMNS);
                            game_started = true; // pass this by reference to update this value in make game in class it has not been made correctly
                            break;
                        case LOAD_GAME:
                            printf("not implemented yet.\n");
                            break;
                        case QUIT:
                            printf("Client has closed the connection\n");
                            exit(EXIT_SUCCESS);
                            break;
                        default:
                            printf("Invalid selection");
                            break;
                    }
                }
            }
            
            // Close child socket connection and exit child
            close(cl_sd);
            exit(EXIT_SUCCESS);
        }
        else
        {
        // Close client connection if internal loop exits
            close(cl_sd); 
        }
                        
    }
    // Close the main socket
    close(sd);

    return 0;
}