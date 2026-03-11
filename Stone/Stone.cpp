// Stone.cpp : Defines the entry point for the application.
//

#include "Stone.h"
#include "Game.h"

#include <SDL3/SDL.h>

using namespace std;

int main()
{
    const int fps = 60;
    const int desiredFrameTime = 1000 / fps; // in milliseconds

    Uint64 ticks = SDL_GetTicks();
    float deltaTime = 0;
    int actualFrameTime;

    Game* game = new Game();
    game->init( "Stone Duel", 800, 600, false );

    while (game->isRunning())
    {
        // Get delta time
        int currentTicks = SDL_GetTicks();
        deltaTime = (currentTicks - ticks) / 1000.0f; // convert to seconds
        ticks = currentTicks;

        game->handleEvents();
        game->update( deltaTime );
        game->render();

        // Frame rate capping
        actualFrameTime = SDL_GetTicks() - ticks;
        if (desiredFrameTime > actualFrameTime)
        {
            SDL_Delay( desiredFrameTime - actualFrameTime );
        }
    }
    cout << "Hello CMake." << endl;
    return 0;
}
