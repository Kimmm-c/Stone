// Stone.cpp : Defines the entry point for the application.
//

#include "Stone.h"
#include "ST_Game.h"

using namespace std;

ST_Game* game;

int main()
{
    game = new ST_Game( "Stone Duel", 800, 600, false, 60 );
    game->init();
    game->run();

    cout << "Hello CMake." << endl;
    return 0;
}
