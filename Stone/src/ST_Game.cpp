#include "ST_Game.h"
#include "ST_TextureManager.h"
#include "ST_MapManager.h"
#include "ST_PhysicsSystem.h"
#include "ST_CollisionSystem.h"
#include "ST_ColliderSyncSystem.h"
#include "ST_SpawnTimerSystem.h"
#include "ST_KeyboardInputSystem.h"
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
    ST_Layer& background = gameplayScene.createLayer();

    ST_MapManager::loadMap
    (
        { assetPath + "maps/background.tmx", 32, 32, false, background }
        , { ST_TextureManager::load( assetPath + "spritesheet.png" ), 2, 2 }
    );

    ST_Layer& midground = gameplayScene.createLayer();
    ST_MapManager::loadMap
    (
        { assetPath + "maps/midground.tmx", 32, 32, true, midground, "tile" }
        , { ST_TextureManager::load( assetPath + "spritesheet.png" ), 2, 2 }
    );

    // create player
    ST_Entity& player = midground.createEntity();
    player.addComponent<Transform>( ST_Vector2D( 10.0f, 10.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    player.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 100.0f );

    SDL_Texture* playerTexture = ST_TextureManager::load( assetPath + "mario.png" );
    SDL_FRect playerSrc{ 0, 0, 32,32 };
    SDL_FRect playerDst{ 0, 0, 64, 64 };
    player.addComponent<Sprite>( playerTexture, playerSrc, playerDst );

    Collider& playerCollider = player.addComponent<Collider>( "player" );
    playerCollider.rect.w = 64;
    playerCollider.rect.h = 64;

    player.addComponent<PlayerTag>();

    // Set up camera
    Camera camera = gameplayScene.createCamera();
    camera.view = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(m_Window->getWidth()), static_cast<float>(m_Window->getHeight()) };
    camera.worldWidth = static_cast<float>(m_Window->getWidth() * 2);
    camera.worldHeight = static_cast<float>(m_Window->getHeight() * 2);

    // Register event handler
    gameplayScene.registerEventHandler<ST_PlayerActionEvent>( playerActionHandler );
    gameplayScene.registerEventHandler<ST_ProjectileEvent>( projectileHandler );
    gameplayScene.registerEventHandler<ST_CollisionEvent>( collisionHandler );

    SDL_Log( "do something" );

    // Set up systems
    gameplayScene.addSystem<ST_KeyboardInputSystem>();
    gameplayScene.addSystem<ST_PhysicsSystem>();
    gameplayScene.addSystem<ST_ColliderSyncSystem>();
    gameplayScene.addSystem<ST_CollisionSystem>();
    gameplayScene.addSystem<ST_SpawnTimerSystem>();

    gameplayScene.registerLayer<
        ST_KeyboardInputSystem
        , ST_PhysicsSystem
        , ST_ColliderSyncSystem
        , ST_CollisionSystem
        , ST_SpawnTimerSystem
    >( midground );

    // Set up time spawner
    // NOTE: This is set up to test collision between projectile and map tile
    // This should be removed when testing is completed
    ST_Entity& spawner( midground.createEntity() );
    Transform transform = spawner.addComponent<Transform>( ST_Vector2D( 10.0f, 0.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    spawner.addComponent<TimedSpawner>( 3.0f, [this, &midground, transform] {
        // create the projectile
        ST_Entity& projectile = midground.createEntity();
        projectile.addComponent<Transform>( ST_Vector2D( transform.position.x, transform.position.y ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
        projectile.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 100.0f );

        SDL_Texture* texture = ST_TextureManager::load( std::string( ASSET_PATH ) + "spritesheet.png" );
        SDL_FRect src{ 0, 0, 32, 32 };
        SDL_FRect dest{ transform.position.x, transform.position.y, 64, 64 };
        projectile.addComponent<Sprite>( texture, src, dest );

        Collider& collision = projectile.addComponent<Collider>( "destructiveProjectile" );

        collision.rect.w = dest.w;
        collision.rect.h = dest.h;

        projectile.addComponent<DestructiveProjectileTag>();
        projectile.addComponent<Projectile>();
                                        } );
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