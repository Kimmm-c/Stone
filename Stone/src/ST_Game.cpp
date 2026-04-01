#include "ST_Game.h"
#include "ST_TextureManager.h"
#include "ST_MapManager.h"
#include "ST_PhysicsSystem.h"
#include "ST_CollisionSystem.h"
#include "ST_ColliderSyncSystem.h"
#include "ST_KeyboardInputSystem.h"
#include "ST_ProjectileSpawnSystem.h"
#include "ST_MovementSystem.h"
#include "ST_TurnManagementSystem.h"
#include "ST_ProjectileDestructionSystem.h"
#include "ST_GameStateSystem.h"
#include "ST_EventHandler.h"

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

    // Set up background layer
    ST_Layer& background = gameplayScene.createLayer();

    ST_Entity& spriteHolder = background.createEntity();
    spriteHolder.addComponent<Transform>( ST_Vector2D( 0.0f, 0.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    SDL_Texture* bgTexture = ST_TextureManager::load( assetPath + "starry-night-bg.jpg" );
    SDL_FRect bgSrc{ 0, 0, 1600, 1200 };
    SDL_FRect bgDst{ 0, 0, 800, 600 };
    spriteHolder.addComponent<Sprite>( bgTexture, bgSrc, bgDst );

    // Set up midground layer
    ST_Layer& midground = gameplayScene.createLayer();
    ST_MapManager::loadMap
    (
        { assetPath + "maps/midground.tmx", 64, 64, true, midground, "tile" }
        , { ST_TextureManager::load( assetPath + "ground.png" ), 1, 1 }
    );

    // create player A
    ST_Entity& playerA = midground.createEntity();
    playerA.addComponent<Transform>( ST_Vector2D( 10.0f, 10.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    playerA.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 150.0f );

    SDL_Texture* playerTexture = ST_TextureManager::load( assetPath + "mario.png" );
    SDL_FRect playerASrc{ 0, 0, 32,32 };
    SDL_FRect playerADst{ 0, 0, 64, 64 };
    playerA.addComponent<Sprite>( playerTexture, playerASrc, playerADst );

    Collider& playerACollider = playerA.addComponent<Collider>( "player" );
    int offset = 5;
    playerACollider.rect.w = 64 - offset;
    playerACollider.rect.h = 64 - offset;

    playerA.addComponent<PlayerTag>( 0 );
    playerA.addComponent<Projectile>();
    playerA.addComponent<Health>( 1000 );

    // create player B
    ST_Entity& playerB = midground.createEntity();
    playerB.addComponent<Transform>( ST_Vector2D( 300.0f, 10.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    playerB.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 150.0f );

    SDL_FRect playerBSrc{ 0, 0, 32,32 };
    SDL_FRect playerBDst{ 0, 0, 64, 64 };
    playerB.addComponent<Sprite>( playerTexture, playerBSrc, playerBDst );

    Collider& playerBCollider = playerB.addComponent<Collider>( "player" );
    playerBCollider.rect.w = 64 - offset;
    playerBCollider.rect.h = 64 - offset;

    playerB.addComponent<PlayerTag>( 1 );
    playerB.addComponent<Projectile>();
    playerB.addComponent<Health>( 1000 );

    // Set up foreground layer (for UI elements)
    ST_Layer& foreground = gameplayScene.createLayer();

    // Create shooting angle indicators

    // Create power bar indicators

    // Create health bars

    // Set up camera
    Camera& camera = gameplayScene.createCamera();
    camera.view = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(m_Window->getWidth()), static_cast<float>(m_Window->getHeight()) };
    camera.worldWidth = 1280.0f;
    camera.worldHeight = 640.0f;

    // Register event handler
    gameplayScene.registerEventHandler<ST_PlayerActionEvent>( playerActionHandler );
    gameplayScene.registerEventHandler<ST_CollisionEvent>( collisionHandler );

    SDL_Log( "do something" );

    // Set up systems
    gameplayScene.addSystem<ST_KeyboardInputSystem>();
    gameplayScene.addSystem<ST_TurnManagementSystem>();
    gameplayScene.addSystem<ST_MovementSystem>();
    gameplayScene.addSystem<ST_ProjectileSpawnSystem>();
    gameplayScene.addSystem<ST_PhysicsSystem>();
    gameplayScene.addSystem<ST_ColliderSyncSystem>();
    gameplayScene.addSystem<ST_CollisionSystem>();
    gameplayScene.addSystem<ST_ProjectileDestructionSystem>();
    gameplayScene.addSystem<ST_GameStateSystem>();

    gameplayScene.registerLayer<
        ST_KeyboardInputSystem
        , ST_TurnManagementSystem
        , ST_MovementSystem
        , ST_ProjectileSpawnSystem
        , ST_PhysicsSystem
        , ST_ColliderSyncSystem
        , ST_CollisionSystem
        , ST_ProjectileDestructionSystem
        , ST_GameStateSystem
    >( midground );
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