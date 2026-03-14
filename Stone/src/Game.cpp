#if 0
#include "Game.h"
#include "Window.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Time.h"

#include <SDL3/SDL.h>

static SDL_Event event; // Initialized only once and accessible within this file only

void Game::init( const char* title, int windowWidth, int windowHeight, bool fullScreen, uint32_t frameRate )
{
    m_Window = std::make_unique<Window>( title, windowWidth, windowHeight, fullScreen );
    m_Time = std::make_unique<Time>( frameRate );

    if (m_Window->exists()) {
        m_Renderer = std::make_unique<Renderer>( m_Window, title );

        if (!m_Renderer->exists()) {
            SDL_LogError( SDL_LOG_PRIORITY_ERROR, SDL_GetError() );
        }
    }
    else {
        SDL_LogError( SDL_LOG_PRIORITY_ERROR, SDL_GetError() );
        return;
    }

    m_IsRunning = true;
}

void Game::run()
{
    while (m_IsRunning) {
        processInput();
        update( m_Time->getDelta() );
        render();

        m_Time->synchronizeFrameRate();
    }
}

void Game::processInput()
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

void Game::update( float delta )
{
    m_SceneManager->update( delta, event );
}

void Game::render()
{
    m_SceneManager->render( m_Renderer );
}
#endif