#include "ST_Scene.h"
#include "ST_SystemManager.h"
#include "ST_AudioManager.h"
#include "ST_MapManager.h"
#include "ST_AssetManager.h"
#include "ST_EventHandler.h"
#include "ST_SceneManager.h"

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
#include "ST_PlayerAnimationSystem.h"
#include "ST_PlayerStateSystem.h"
#include "ST_AudioSystem.h"
#include "ST_MouseInputSystem.h"

static std::string assetPath = std::string( ASSET_PATH );

ST_Entity& ST_Scene::createPlayer(
    ST_Layer& layer
    , const ST_GameMetadata& meta
    , int playerId
    , const ST_Vector2D& pos
    , const std::string& animName
    , const std::string& animPath
    , const std::string& texturePath
)
{
    ST_Entity& player = layer.createEntity();

    player.addComponent<Transform>( pos, ST_Vector2D( 0, 0 ), 0.0f, 1.0f );
    player.addComponent<Velocity>( ST_Vector2D( 0, 0 ), meta.playerSpeed );

    auto& collider = player.addComponent<Collider>( "player" );
    collider.rect.w = meta.colliderSize - meta.colliderOffset;
    collider.rect.h = meta.colliderSize;

    player.addComponent<PlayerTag>( playerId );
    player.addComponent<Projectile>( playerId );
    player.addComponent<Health>( meta.healthRange );
    player.addComponent<PlayerActionFlags>();
    player.addComponent<PlayerStateComponent>();

    ST_AssetManager::loadAnimation( animName.c_str(), animPath );
    Animation anim = ST_AssetManager::getAnimation( animName.c_str() );
    player.addComponent<Animation>( anim );

    SDL_Texture* tex = ST_TextureManager::load( texturePath );
    SDL_FRect src = anim.clips[anim.currentClip].frameIndices[0];

    player.addComponent<Sprite>( tex, src, meta.playerSpriteDest );

    return player;
}

void ST_Scene::createShootingAngleUI(
    ST_Layer& layer
    , SDL_Texture* pointerTexture
    , SDL_Texture* frameTexture
    , const ST_GameMetadata& meta
    , const ST_Vector2D& playerPos
    , int id
)
{
    ST_Entity& pointer = layer.createEntity();
    ST_Entity& frame = layer.createEntity();

    float pointerX = (playerPos.x + 64.0f) / 2.0f;
    float pointerY = (playerPos.y + 64.0f) / 2.0f;
    float frameX = playerPos.x - 64.0f;
    float frameY = playerPos.y - 64.0f;
    SDL_FRect pointerSrc{ 0, 0, meta.pointerTexSize.x, meta.pointerTexSize.y };
    SDL_FRect frameSrc{ 0, 0, meta.frameTexSize.x, meta.frameTexSize.y };

    pointer.addComponent<Sprite>( pointerTexture, pointerSrc, pointerSrc );
    frame.addComponent<Sprite>( frameTexture, frameSrc, frameSrc );
    pointer.addComponent<Transform>( ST_Vector2D( pointerX, pointerY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    frame.addComponent<Transform>( ST_Vector2D( frameX, frameY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    pointer.addComponent<AnglePointerUI>( id, meta.pointerDistFromPlayer );
    frame.addComponent<AngleFrameUITag>( id );
}

void ST_Scene::createPowerBar(
    ST_Layer& layer
    , SDL_Texture* gradientTexture
    , SDL_Texture* outlineTexture
    , const ST_GameMetadata& meta
    , float offsetX
    , float offsetY
    , int id
    , bool flip )
{
    ST_Entity& barGradient = layer.createEntity();
    ST_Entity& barOutline = layer.createEntity();

    SDL_FRect gradientTextureSrc{ 0, 0, 0, 0 }; // power should be empty when game starts
    SDL_FRect gradientTextureDest{ 0, 0, meta.powerBarDim.x, meta.powerBarDim.y };

    SDL_FRect outlineTextureSrc{ 0, 0, meta.powerBarDim.x, meta.powerBarDim.y };

    barGradient.addComponent<Sprite>( gradientTexture, gradientTextureSrc, gradientTextureDest, meta.powerBarDim, flip );
    barOutline.addComponent<Sprite>( outlineTexture, outlineTextureSrc, outlineTextureSrc );
    barGradient.addComponent<Transform>( ST_Vector2D( offsetX, offsetY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    barOutline.addComponent<Transform>( ST_Vector2D( offsetX, offsetY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    barGradient.addComponent<PowerBarTag>( id );

    ST_Vector2D fixedPowerUIPosition{ offsetX, offsetY };
    barGradient.addComponent<ScreenSpaceUI>( fixedPowerUIPosition );
    barOutline.addComponent<ScreenSpaceUI>( fixedPowerUIPosition );
}

ST_Entity& ST_Scene::createHealthBar(
    ST_Layer& layer
    , SDL_Texture* healthTexture
    , const ST_GameMetadata& meta
    , float offsetX
    , float offsetY
    , int id
    , bool flip
)
{
    ST_Entity& healthBar = layer.createEntity();

    SDL_FRect healthDim{ 0, 0, meta.healthSize.x, meta.healthSize.y };
    healthBar.addComponent<Sprite>( healthTexture, healthDim, healthDim, meta.healthSize, flip );
    healthBar.addComponent<Transform>( ST_Vector2D( offsetX, offsetY ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    healthBar.addComponent<HealthUITag>( id );

    ST_Vector2D fixedHealthUIPosition{ offsetX, offsetY };
    healthBar.addComponent<ScreenSpaceUI>( fixedHealthUIPosition );

    return healthBar;
}

ST_Entity& ST_Scene::createButton(
    ST_Layer& layer
    , ST_Entity& parent
    , const std::string& texturePath
    , const SDL_FRect& parentRect
    , float offsetX
    , float offsetY
    , const ST_GameMetadata& meta
    , std::function<void()> onClick
)
{
    ST_Entity& button = layer.createEntity();

    SDL_Texture* texture = ST_TextureManager::load( texturePath );
    SDL_FRect src{ 0, 0, meta.buttonW, meta.buttonH };
    SDL_FRect dest = ST_RenderHelper::getCenterDest( src, parentRect );
    dest = ST_RenderHelper::getScaledDest( dest, 0.25 );
    dest.x += offsetX;
    dest.y += offsetY;
    button.addComponent<Sprite>(
        texture,
        src,
        dest,
        ST_Vector2D( meta.buttonW, meta.buttonH ),
        false,
        RenderLayer::UI,
        false // not visible during gameplay
    );
    Transform& transform = button.addComponent<Transform>( ST_Vector2D( dest.x, dest.y ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
    button.addComponent<Parent>( &parent );
    button.addComponent<Collider>( "ui", dest );

    Clickable& clickable = button.addComponent<Clickable>();

    // shrink slightly when pressed
    clickable.onPressed = [&transform]() {
        transform.scale = 0.75;
        };

    // restore button's size when mouse leaves
    clickable.onCancel = [&transform]() {
        transform.scale = 1.0f;
        };

    // restore button's size + exit game
    clickable.onReleased = [onClick, &transform]() {
        transform.scale = 1.0f;
        onClick();
        };

    if (parent.hasComponent<Children>())
        parent.getComponent<Children>().children.push_back( &button );
    else
        SDL_Log( "Failed to find children component!" );

    return button;
}

ST_Scene::ST_Scene( const ST_GameMetadata& meta )
{
    // Set up camera
    Camera& camera = createCamera();
    camera.view = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(meta.windowW), static_cast<float>(meta.windowH) };
    camera.worldWidth = meta.windowW * 2;
    camera.worldHeight = meta.windowH;

    // Set up sound effects
    ST_AudioManager::loadAudio( "whoosh", assetPath + "audios/sfx/whoosh.mp3" );
    ST_AudioManager::loadAudio( "collision", assetPath + "audios/sfx/collision.mp3" );
    ST_AudioManager::loadAudio( "horrorBg", assetPath + "audios/music/bg-horror-theme-music.mp3" );
    // TODO: Tailor bg music to scene?
    ST_AudioManager::stopMusic();   // stop music on new match
    ST_AudioManager::playMusic( "horrorBg" );

    // Set up background layer
    ST_Layer& background = createLayer();

    ST_Entity& spriteHolder = background.createEntity();
    SDL_Texture* bgTexture = ST_TextureManager::load( meta.bgPath );
    SDL_FRect bgSrc{ 0, 0, 3240, 2160 };
    SDL_FRect bgDst{ 0, 0, meta.windowW * 2, meta.windowH * 2 };
    spriteHolder.addComponent<Sprite>( bgTexture, bgSrc, bgDst );
    spriteHolder.addComponent<Transform>( ST_Vector2D( 0.0f, 0.0f ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    // Set up midground layer
    ST_Layer& midground = createLayer();
    ST_MapManager::loadMap
    (
        { meta.mapPath, 64, 64, true, midground, "tile" }
        , { ST_TextureManager::load( assetPath + "ground.png" ), 1, 1 }
    );

    // Create gamestate entity
    ST_Entity& gamestate = midground.createEntity();
    gamestate.addComponent<GameStateComponent>();

    // Create players
    int playerAid = 0;
    int playerBid = 1;

    // create player A
    ST_Entity& playerA = createPlayer(
        midground
        , meta
        , playerAid
        , meta.playerASpawn
        , "bald"
        , assetPath + "animations/bald-walk-animation.xml"
        , assetPath + "animations/bald-walk-animation.png"
    );

    // create player B
    ST_Entity& playerB = createPlayer(
        midground
        , meta
        , playerBid
        , meta.playerBSpawn
        , "red-eye"
        , assetPath + "animations/red-eye-animation.xml"
        , assetPath + "animations/red-eye-animation.png"
    );

    // Create shooting angle UI elements
    SDL_Texture* pointerTexture = ST_TextureManager::load( assetPath + "angle-pointer.PNG" );
    SDL_Texture* frameTexture = ST_TextureManager::load( assetPath + "angle-frame.PNG" );

    // Player A's shooting angle UI
    createShootingAngleUI(
        midground
        , pointerTexture
        , frameTexture
        , meta
        , meta.playerASpawn
        , playerAid
    );

    // Player B's shooting angle UI
    createShootingAngleUI(
        midground
        , pointerTexture
        , frameTexture
        , meta
        , meta.playerBSpawn
        , playerBid
    );

    // Create shooting power UI elements
    float barWidthOffsetA = 5.0f;
    float barWidthOffsetB = meta.windowW - (meta.powerBarDim.x + 5.0f);
    float barHeightOffset = 10.0f;

    SDL_Texture* gradientTexture = ST_TextureManager::load( assetPath + "power-bar-gradient.PNG" );
    SDL_Texture* outlineTexture = ST_TextureManager::load( assetPath + "power-bar-outline.PNG" );

    // Player A's power bar
    createPowerBar(
        midground
        , gradientTexture
        , outlineTexture
        , meta
        , barWidthOffsetA
        , barHeightOffset
        , playerAid
        , false
    );

    // Player B's power bar
    createPowerBar(
        midground
        , gradientTexture
        , outlineTexture
        , meta
        , barWidthOffsetB
        , barHeightOffset
        , playerBid
        , true
    );

    // Create health bars
    float healthWidthOffsetA = 5.0f;
    float healthWidthOffsetB = meta.windowW - (meta.powerBarDim.x + 5.0f);
    float healthHeightOffset = 10.0f + (barHeightOffset + meta.powerBarDim.y);

    SDL_Texture* healthTexture = ST_TextureManager::load( assetPath + "health.PNG" );

    // Create playerA's health UI
    ST_Entity& healthBarA = createHealthBar(
        midground
        , healthTexture
        , meta
        , healthWidthOffsetA
        , healthHeightOffset
        , playerAid
        , false
    );

    // Create playerB's health UI
    ST_Entity& healthBarB = createHealthBar(
        midground
        , healthTexture
        , meta
        , healthWidthOffsetB
        , healthHeightOffset
        , playerBid
        , true
    );

    // Register event handler
    registerEventHandler<ST_PlayerActionEvent>( playerActionHandler );
    registerEventHandler<ST_CollisionEvent>( collisionHandler );
    registerEventHandler<ST_GameOverEvent>( gameOverHandler );
    registerEventHandler<ST_MouseInteractionEvent>( mouseInteractionHandler );

    // Set up systems
    addSystem<ST_KeyboardInputSystem>();
    addSystem<ST_MouseInputSystem>();
    addSystem<ST_TurnManagementSystem>();
    addSystem<ST_MovementSystem>();
    addSystem<ST_ProjectileSpawnSystem>();
    addSystem<ST_PhysicsSystem>();
    addSystem<ST_PowerBarUISyncSystem>();
    addSystem<ST_ShootingAngleUISyncSystem>();
    addSystem<ST_ColliderSyncSystem>();
    addSystem<ST_CollisionSystem>();
    addSystem<ST_HealthDamageSystem>();
    addSystem<ST_ProjectileDestructionSystem>();
    addSystem<ST_PlayerStateSystem>();
    addSystem<ST_PlayerAnimationSystem>();
    addSystem<ST_AnimationSystem>();
    addSystem<ST_HealthUISyncSystem>();
    addSystem<ST_CameraSystem>();
    addSystem<ST_ScreenUISystem>();
    addSystem<ST_AudioSystem>();
    addSystem<ST_GameStateSystem>();

    registerLayer <
        ST_KeyboardInputSystem
        , ST_MouseInputSystem
        , ST_TurnManagementSystem
        , ST_MovementSystem
        , ST_ProjectileSpawnSystem
        , ST_PhysicsSystem
        , ST_PowerBarUISyncSystem
        , ST_ShootingAngleUISyncSystem
        , ST_ColliderSyncSystem
        , ST_CollisionSystem
        , ST_HealthDamageSystem
        , ST_ProjectileDestructionSystem
        , ST_PlayerStateSystem
        , ST_PlayerAnimationSystem
        , ST_AnimationSystem
        , ST_HealthUISyncSystem
        , ST_CameraSystem
        , ST_ScreenUISystem
        , ST_AudioSystem
        , ST_GameStateSystem
    > (midground);

    // Create end game overlay
    // Create overlay panel
    float overlayW = 350.0f;
    float overlayH = 251.0f;
    ST_Entity& overlay = midground.createEntity();
    SDL_Texture* overlayTexture = ST_TextureManager::load( assetPath + "settings.jpg" );
    SDL_FRect overlaySrc{ 0, 0, overlayW, overlayH };
    SDL_FRect overlayDest{ 0, 0, overlayW, overlayH };
    overlayDest = ST_RenderHelper::getCenterDest( overlayDest, SDL_FRect( 0, 0, meta.windowW, meta.windowH ) );
    overlayDest = ST_RenderHelper::getScaledDest( overlayDest, 2.0f );
    overlay.addComponent<Sprite>(
        overlayTexture,
        overlaySrc,
        overlayDest,
        ST_Vector2D( overlayW, overlayH ),
        false,
        RenderLayer::UI,
        false // not visible during gameplay
    );
    overlay.addComponent<Transform>( ST_Vector2D( overlayDest.x, overlayDest.y ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );

    auto& overlayChildren = overlay.addComponent<Children>();


    // create overlay panel buttons
    // Set up exit button
    ST_Entity& exit = createButton(
        midground
        , overlay
        , assetPath + "red-button.PNG"
        , overlayDest
        , -meta.buttonOffsetX   // move to the left
        , meta.buttonOffsetY
        , meta
        , []() { ST_SceneManager::requestQuit(); }
    );

    // Set up rematch button
    ST_Entity& rematch = createButton(
        midground
        , overlay
        , assetPath + "blue-button.PNG"
        , overlayDest
        , meta.buttonOffsetX   // move to the right
        , meta.buttonOffsetY
        , meta
        , []() { ST_SceneManager::changeSceneDeferred( "scene2" ); }
    );

    // Create place holder to display the winner
    ST_Entity& winnerPlaceholder = midground.createEntity();
    winnerPlaceholder.addComponent<Parent>( &overlay );
    winnerPlaceholder.addComponent<WinnerTag>();
    overlayChildren.children.push_back( &winnerPlaceholder );
}

ST_Layer& ST_Scene::createLayer()
{
    m_Layers.emplace_back( std::make_unique<ST_Layer>() );

    return *m_Layers.back();
}

Camera& ST_Scene::createCamera()
{
    m_Camera = std::make_unique<Camera>();

    return *m_Camera;
}

void ST_Scene::update( float delta, SDL_Event& event )
{
    m_SystemManager->update( { delta, event, *m_EventManager.get(), *m_Camera.get() } );

    m_EventManager->process();

    for (auto& layer : m_Layers)
        layer->cleanup();
}

void ST_Scene::render()
{
    for (auto& layer : m_Layers)
        layer->render( m_Camera.get() );
}