#include "ST_Game.h"
#include "ST_SceneManager.h"

#include <SDL3/SDL.h>
#include <iostream>

static SDL_Event event; // Initialized only once and accessible within this file only

ST_Game::ST_Game( const char* title, int windowWidth, int windowHeight, bool fullScreen, uint32_t frameRate )
{
    m_Window = std::make_unique<ST_Window>( title, windowWidth, windowHeight, fullScreen );
    m_Time = std::make_unique<ST_Time>( frameRate );

    if (m_Window->exists()) {
        m_Renderer = std::make_unique<ST_Renderer>( m_Window.get(), nullptr );

        if (!m_Renderer->exists()) {
            SDL_LogError( SDL_LOG_PRIORITY_ERROR, SDL_GetError() );
            return;
        }
    }
    else {
        SDL_LogError( SDL_LOG_PRIORITY_ERROR, SDL_GetError() );
        return;
    }

    m_IsRunning = true;
}

void ST_Game::init()
{
    SDL_Log( "setting up the game.." );
}

void ST_Game::run()
{
    while (m_IsRunning) {
        processInput();
        update( m_Time->getDelta() );
        render();

        m_Time->synchronizeFrameRate();
    }
}

void ST_Game::processInput()
{
    SDL_PollEvent( &event ); // if there is a pending event, store it in event variable and remove it from the queue
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
        m_IsRunning = false;
        break;
    default:
        break;
    }
}

void ST_Game::update( float delta )
{
    //m_SceneManager->update( delta, event );
    std::cout << "updating a frame..." << std::endl;
}

void ST_Game::render()
{
    //m_SceneManager->render( m_Renderer );
    std::cout << "rendering a frame..." << std::endl;
}