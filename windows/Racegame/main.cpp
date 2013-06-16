#include <../../src/headers/Game.h>

int main( void )
{
	Game * game = new Game();
    game->build();
    game->run();
    game->stop();

	return 0;
}