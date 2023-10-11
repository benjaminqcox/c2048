// #include "userInput.h"
#include "game.h"

int main()
{
    // Add the ability to reverse a move. Maybe store up to x gamestates to revert to (5 could be a good value)
    // Use linked lists to store the games as that way I won't need to shift 5 elements in the array every time (2 instead)
    game_t *game = makeGame(MAX_ROWS, MAX_COLUMNS);
    playGame(game);
	return 0;
}


