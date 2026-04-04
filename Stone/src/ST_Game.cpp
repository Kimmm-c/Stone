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
#include "ST_PowerBarUISyncSystem.h"
#include "ST_ShootingAngleUISyncSystem.h"
#include "ST_HealthUISyncSystem.h"
#include "ST_HealthDamageSystem.h"
#include "ST_GameStateSystem.h"
#include "ST_CameraSystem.h"
#include "ST_ScreenUISystem.h"
#include "ST_AnimationSystem.h"
#include "ST_EventHandler.h"
#include "ST_AssetManager.h"

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

    // Set up camera
    Camera& camera = gameplayScene.createCamera();
    camera.view = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(m_Window->getWidth()), static_cast<float>(m_Window->getHeight()) };
    camera.worldWidth = m_Window->getWidth() * 2;
    camera.worldHeight = m_Window->getHeight();

    // Set up sound effects
    m_AudioManager->loadAudio( "whoosh", assetPath + "audios/sfx/whoosh.mp3" );
    m_AudioManager->loadAudio( "collision", assetPath + "audios/sfx/collision.mp3" );
    m_AudioManager->playSfx( "collision" );

    // Set up background layer
    ST_Layer& background = gameplayScene.createLayer();

    ST_Entity& spriteHolder = background.createEntity();
    SDL_Texture* bgTexture = ST_TextureManager::load( assetPath + "castle-bg.png" );
    SDL_FRect bgSrc{ 0, 0, 3240, 2160 };
    SDL_FRect bgDst{ 0, 0, m_Window->getWidth() * 2, m_Window->getHeight() * 2 };
    spriteHolder.addComponent<Sprite>( bgTexture, bgSrc, bgDst );
    spriteHolder.addComponent<Transform>( ST_Vector2D( 0.0f, 0.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    // Set up midground layer
    ST_Layer& midground = gameplayScene.createLayer();
    ST_MapManager::loadMap
    (
        { assetPath + "maps/midground.tmx", 64, 64, true, midground, "tile" }
        , { ST_TextureManager::load( assetPath + "ground.png" ), 1, 1 }
    );

    // Create players
    int playerAid = 0;
    int playerBid = 1;
    ST_Vector2D healthRange{ 0.0f, 5000.0f };

    // create player A
    ST_Entity& playerA = midground.createEntity();
    ST_Vector2D playerAPos{ 10.0f, 10.0f };
    playerA.addComponent<Transform>( playerAPos, ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    playerA.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 150.0f );

    Collider& playerACollider = playerA.addComponent<Collider>( "player" );
    int offset = 10;
    playerACollider.rect.w = 64 - offset;
    playerACollider.rect.h = 64 - offset;

    playerA.addComponent<PlayerTag>( playerAid );
    playerA.addComponent<Projectile>( playerAid );
    playerA.addComponent<Health>( healthRange );

    const char* animationNameA = "bald";
    ST_AssetManager::loadAnimation( animationNameA, assetPath + "animations/bald-walk-animation.xml" );
    Animation animationA = ST_AssetManager::getAnimation( animationNameA );
    playerA.addComponent<Animation>( animationA );

    SDL_Texture* playerATexture = ST_TextureManager::load( assetPath + "animations/bald-walk-animation.png" );
    SDL_FRect playerASrc = animationA.clips[animationA.currentClip].frameIndices[0];
    SDL_FRect playerADst{ 0, 0, 64, 64 };
    playerA.addComponent<Sprite>( playerATexture, playerASrc, playerADst );

    // create player B
    ST_Entity& playerB = midground.createEntity();
    ST_Vector2D playerBPos{ 300.0f, 10.0f };
    playerB.addComponent<Transform>( playerBPos, ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    playerB.addComponent<Velocity>( ST_Vector2D( 0.0f, 0.0f ), 150.0f );

    Collider& playerBCollider = playerB.addComponent<Collider>( "player" );
    playerBCollider.rect.w = 64 - offset;
    playerBCollider.rect.h = 64 - offset;

    playerB.addComponent<PlayerTag>( playerBid );
    playerB.addComponent<Projectile>( playerBid );
    playerB.addComponent<Health>( healthRange );

    const char* animationNameB = "red-eye";
    ST_AssetManager::loadAnimation( animationNameB, assetPath + "animations/red-eye-animation.xml" );
    Animation animationB = ST_AssetManager::getAnimation( animationNameB );
    playerB.addComponent<Animation>( animationB );

    SDL_Texture* playerBTexture = ST_TextureManager::load( assetPath + "animations/red-eye-animation.png" );
    SDL_FRect playerBSrc = animationB.clips[animationB.currentClip].frameIndices[0];
    SDL_FRect playerBDst{ 0, 0, 64, 64 };
    playerB.addComponent<Sprite>( playerBTexture, playerBSrc, playerBDst );

    // Set up foreground layer (for UI elements)
    ST_Layer& foreground = gameplayScene.createLayer();

    // Create shooting angle UI elements
    ST_Entity& anglePointertA = midground.createEntity();
    ST_Entity& angleFrameA = midground.createEntity();
    ST_Entity& anglePointertB = midground.createEntity();
    ST_Entity& angleFrameB = midground.createEntity();

    ST_Vector2D pointerTexDimension{ 7.0f, 7.0f };
    ST_Vector2D frameTexDimension{ 128.0f, 128.0f };

    SDL_Texture* pointerTexture = ST_TextureManager::load( assetPath + "angle-pointer.PNG" );
    SDL_FRect pointerDim{ 0, 0, pointerTexDimension.x, pointerTexDimension.y };

    SDL_Texture* frameTexture = ST_TextureManager::load( assetPath + "angle-frame.PNG" );
    SDL_FRect frameDim{ 0, 0, frameTexDimension.x, frameTexDimension.y };

    float distFromPlayer = 50.0f;

    // Player A's shooting angle UI
    float pointerAX = (playerAPos.x + 64.0f) / 2.0f;
    float pointerAY = (playerAPos.y + 64.0f) / 2.0f;
    float frameAX = playerAPos.x - 64.0f;
    float frameAY = playerAPos.y - 64.0f;

    anglePointertA.addComponent<Sprite>( pointerTexture, pointerDim, pointerDim );
    angleFrameA.addComponent<Sprite>( frameTexture, frameDim, frameDim );
    anglePointertA.addComponent<Transform>( ST_Vector2D( pointerAX, pointerAY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    angleFrameA.addComponent<Transform>( ST_Vector2D( frameAX, frameAY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    anglePointertA.addComponent<AnglePointerUI>( playerAid, distFromPlayer );
    angleFrameA.addComponent<AngleFrameUITag>( playerAid );

    // Player B's shooting angle UI
    float pointerBX = (playerBPos.x + 64.0f) / 2.0f;
    float pointerBY = (playerBPos.y + 64.0f) / 2.0f;
    float frameBX = playerBPos.x - 64.0f;
    float frameBY = playerBPos.y - 64.0f;

    anglePointertB.addComponent<Sprite>( pointerTexture, pointerDim, pointerDim );
    angleFrameB.addComponent<Sprite>( frameTexture, frameDim, frameDim );
    anglePointertB.addComponent<Transform>( ST_Vector2D( pointerBX, pointerBY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    angleFrameB.addComponent<Transform>( ST_Vector2D( frameBX, frameBY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    anglePointertB.addComponent<AnglePointerUI>( playerBid, distFromPlayer );
    angleFrameB.addComponent<AngleFrameUITag>( playerBid );

    // Create shooting power UI elements
    ST_Entity& barGradientA = midground.createEntity();
    ST_Entity& barOutlineA = midground.createEntity();
    ST_Entity& barGradientB = midground.createEntity();
    ST_Entity& barOutlineB = midground.createEntity();

    float barSrcWidth = 416.0f;
    float barSrcHeight = 32.0f;
    float barWidthOffsetA = 5.0f;
    float barWidthOffsetB = m_Window->getWidth() - (barSrcWidth + 5.0f);
    float barHeightOffset = 10.0f;

    SDL_Texture* gradientTexture = ST_TextureManager::load( assetPath + "power-bar-gradient.PNG" );
    SDL_FRect gradientTextureSrc{ 0, 0, 0, 0 };
    SDL_FRect gradientTextureDest{ 0, 0, barSrcWidth,barSrcHeight };

    SDL_Texture* outlineTexture = ST_TextureManager::load( assetPath + "power-bar-outline.PNG" );
    SDL_FRect outlineTextureSrc{ 0, 0, barSrcWidth,barSrcHeight };
    SDL_FRect outlineTextureDest{ 0, 0, barSrcWidth,barSrcHeight };

    ST_Vector2D barFixedDimension{ barSrcWidth, barSrcHeight };

    // Player A's power bar
    barGradientA.addComponent<Sprite>( gradientTexture, gradientTextureSrc, gradientTextureDest, barFixedDimension );
    barOutlineA.addComponent<Sprite>( outlineTexture, outlineTextureSrc, outlineTextureDest );
    barGradientA.addComponent<Transform>( ST_Vector2D( barWidthOffsetA, barHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    barOutlineA.addComponent<Transform>( ST_Vector2D( barWidthOffsetA, barHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    barGradientA.addComponent<PowerBarTag>( playerAid );

    ST_Vector2D fixedPowerUIPositionA{ barWidthOffsetA, barHeightOffset };
    barGradientA.addComponent<ScreenSpaceUI>( fixedPowerUIPositionA );
    barOutlineA.addComponent<ScreenSpaceUI>( fixedPowerUIPositionA );

    // Player B's power bar
    barGradientB.addComponent<Sprite>( gradientTexture, gradientTextureSrc, gradientTextureDest, barFixedDimension );
    barOutlineB.addComponent<Sprite>( outlineTexture, outlineTextureSrc, outlineTextureDest );
    barGradientB.addComponent<Transform>( ST_Vector2D( barWidthOffsetB, barHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    barOutlineB.addComponent<Transform>( ST_Vector2D( barWidthOffsetB, barHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    barGradientB.addComponent<PowerBarTag>( playerBid );

    ST_Vector2D fixedPowerUIPositionB{ barWidthOffsetB, barHeightOffset };
    barGradientB.addComponent<ScreenSpaceUI>( fixedPowerUIPositionB );
    barOutlineB.addComponent<ScreenSpaceUI>( fixedPowerUIPositionB );

    // Create health bars
    ST_Entity& healthBarA = midground.createEntity();
    ST_Entity& healthBarB = midground.createEntity();

    ST_Vector2D fixedHealthDim{ 416.0f, 16.0f };
    float healthWidthOffsetA = 5.0f;
    float healthWidthOffsetB = m_Window->getWidth() - (barSrcWidth + 5.0f);
    float healthHeightOffset = 10.0f + (barHeightOffset + barSrcHeight);

    SDL_Texture* healthTexture = ST_TextureManager::load( assetPath + "health.PNG" );
    SDL_FRect healthDim{ 0, 0, fixedHealthDim.x, fixedHealthDim.y };

    // Create playerA's health UI
    healthBarA.addComponent<Sprite>( healthTexture, healthDim, healthDim, fixedHealthDim );
    healthBarA.addComponent<Transform>( ST_Vector2D( healthWidthOffsetA, healthHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    healthBarA.addComponent<HealthUITag>( playerAid );

    ST_Vector2D fixedHealthUIPositionA{ healthWidthOffsetA, healthHeightOffset };
    healthBarA.addComponent<ScreenSpaceUI>( fixedHealthUIPositionA );

    // Create playerB's health UI
    healthBarB.addComponent<Sprite>( healthTexture, healthDim, healthDim, fixedHealthDim, true );
    healthBarB.addComponent<Transform>( ST_Vector2D( healthWidthOffsetB, healthHeightOffset ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    healthBarB.addComponent<HealthUITag>( playerBid );

    ST_Vector2D fixedHealthUIPositionB{ healthWidthOffsetB, healthHeightOffset };
    healthBarB.addComponent<ScreenSpaceUI>( fixedHealthUIPositionB );

    // Register event handler
    gameplayScene.registerEventHandler<ST_PlayerActionEvent>( playerActionHandler );
    gameplayScene.registerEventHandler<ST_CollisionEvent>( collisionHandler );

    SDL_Log( "do something" );

    // Set up systems
    gameplayScene.addSystem<ST_KeyboardInputSystem>();
    gameplayScene.addSystem<ST_TurnManagementSystem>();
    gameplayScene.addSystem<ST_MovementSystem>();
    gameplayScene.addSystem<ST_AnimationSystem>();
    gameplayScene.addSystem<ST_ProjectileSpawnSystem>();
    gameplayScene.addSystem<ST_PhysicsSystem>();
    gameplayScene.addSystem<ST_PowerBarUISyncSystem>();
    gameplayScene.addSystem<ST_ShootingAngleUISyncSystem>();
    gameplayScene.addSystem<ST_ColliderSyncSystem>();
    gameplayScene.addSystem<ST_CollisionSystem>();
    gameplayScene.addSystem<ST_HealthDamageSystem>();
    gameplayScene.addSystem<ST_ProjectileDestructionSystem>();
    gameplayScene.addSystem<ST_HealthUISyncSystem>();
    gameplayScene.addSystem<ST_CameraSystem>();
    gameplayScene.addSystem<ST_ScreenUISystem>();
    gameplayScene.addSystem<ST_GameStateSystem>();

    gameplayScene.registerLayer<
        ST_KeyboardInputSystem
        , ST_TurnManagementSystem
        , ST_MovementSystem
        , ST_AnimationSystem
        , ST_ProjectileSpawnSystem
        , ST_PhysicsSystem
        , ST_PowerBarUISyncSystem
        , ST_ShootingAngleUISyncSystem
        , ST_ColliderSyncSystem
        , ST_CollisionSystem
        , ST_HealthDamageSystem
        , ST_ProjectileDestructionSystem
        , ST_HealthUISyncSystem
        , ST_CameraSystem
        , ST_ScreenUISystem
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