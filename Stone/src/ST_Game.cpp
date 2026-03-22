#include "ST_Game.h"
#include "ST_TextureManager.h"
#include "ST_MapManager.h"

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
    // create a scene
    ST_Scene& gameplayScene = m_SceneManager->loadScene( "gameplay", true );
    ST_Layer& background = gameplayScene.createLayer();

    ST_MapManager::loadMap( { assetPath + "maps/background.tmx", 32, 32, false, background }
    , ST_TextureManager::load( assetPath + "spritesheet.png" ) );

    ST_Layer& midground = gameplayScene.createLayer();
    ST_MapManager::loadMap( { assetPath + "maps/midground.tmx", 32, 32, true, midground }
    , ST_TextureManager::load( assetPath + "spritesheet.png" ) );

    // create player
    ST_Entity& player = midground.createEntity();
    player.addComponent<Transform>( ST_Vector2D( 10.0f, 10.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    player.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 100.0f );

    SDL_Texture* playerTexture = ST_TextureManager::load( assetPath + "mario.png" );
    SDL_FRect playerSrc{ 0, 0, 32,32 };
    SDL_FRect playerDst{ 0, 0, 64, 64 };
    player.addComponent<Sprite>( playerTexture, playerSrc, playerDst );

    // Set up camera
    Camera camera = gameplayScene.createCamera();
    camera.view = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(m_Window->getWidth()), static_cast<float>(m_Window->getHeight()) };
    camera.worldWidth = static_cast<float>(m_Window->getWidth() * 2);
    camera.worldHeight = static_cast<float>(m_Window->getHeight() * 2);

    SDL_Log( "do something" );
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
    m_SceneManager->update( delta, event );
}

void ST_Game::render()
{
    SDL_RenderClear( ST_TextureManager::getRenderer()->getNativeRenderer() );
    m_SceneManager->render();
    SDL_RenderPresent( ST_TextureManager::getRenderer()->getNativeRenderer() );
}