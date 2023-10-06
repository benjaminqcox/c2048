// #include "userInput.h"
#include "game.h"

int main()
{	
    game_t *game = makeGame(MAX_ROWS, MAX_COLUMNS);
    playGame(game);
	return 0;
}


