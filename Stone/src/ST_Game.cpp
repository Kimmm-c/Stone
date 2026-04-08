#include "ST_Game.h"
#include "ST_TextureManager.h"
#include "ST_Scene.h"


#include <SDL3/SDL.h>
#include <iostream>

static SDL_Event event; // Initialized only once and accessible within this file only
static std::string assetPath = std::string( ASSET_PATH );


ST_Game::ST_Game( const char* title, int windowWidth, int windowHeight, bool fullScreen, uint32_t frameRate )
{
    m_Window = std::make_unique<ST_Window>( title, windowWidth, windowHeight, fullScreen );
    m_Time = std::make_unique<ST_Time>( frameRate );

    if (m_Window->exists()) {
        if (!ST_TextureManager::createRenderer( *m_Window.get() ).getNativeRenderer()) {
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
    ST_AudioManager::init();

    // map 1
    ST_GameMetadata meta1;

    meta1.windowW = m_Window->getWidth();
    meta1.windowH = m_Window->getHeight();

    meta1.mapPath = assetPath + "maps/midground.tmx";
    meta1.bgPath = assetPath + "castle-bg.png";

    std::string scene1 = "scene1";
    ST_SceneManager::loadScene( scene1, meta1 );
    ST_SceneManager::changeSceneDeferred( scene1 );

    // map 2
    ST_GameMetadata meta2;

    meta2.windowW = m_Window->getWidth();
    meta2.windowH = m_Window->getHeight();

    meta2.mapPath = assetPath + "maps/map2.tmx";
    meta2.bgPath = assetPath + "terrace-bg.png";

    std::string scene2 = "scene2";
    ST_SceneManager::loadScene( scene2, meta2 );
}

void ST_Game::run()
{
    while (m_IsRunning) {
        processInput();
        update( m_Time->getDelta() );
        render();

        if (ST_SceneManager::m_ShouldQuit)
            m_IsRunning = false;

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
    ST_SceneManager::update( delta, event );
}

void ST_Game::render()
{
    SDL_RenderClear( ST_TextureManager::getRenderer()->getNativeRenderer() );
    ST_SceneManager::render();
    SDL_RenderPresent( ST_TextureManager::getRenderer()->getNativeRenderer() );
}

void ST_Game::restart()
{
    // TODO: restart the game
}