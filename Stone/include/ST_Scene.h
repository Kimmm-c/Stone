#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "ST_Layer.h"
#include "ST_ISystem.h"
#include "ST_SystemManager.h"
#include "ST_EventManager.h"


class ST_Renderer;

struct ST_GameMetadata
{
    // Window / world
    int worldWidthMultiplier = 2;
    int windowW;
    int windowH;

    // Player
    float playerSpeed = 150.0f;
    ST_Vector2D playerASpawn{ 10.0f, 10.0f };
    ST_Vector2D playerBSpawn{ 500.0f, 10.0f };
    ST_Vector2D healthRange{ 0.0f, 1000.0f };

    // Player sprite
    SDL_FRect playerSpriteDest{ 0, 0, 128, 128 };

    // Player Collider
    int colliderOffset = 20;
    int colliderSize = 64;

    // UI - angle
    float pointerDistFromPlayer = 50.0f;
    ST_Vector2D pointerTexSize{ 7.0f, 7.0f };
    ST_Vector2D frameTexSize{ 128.0f, 128.0f };

    // Bar offset and padding
    float barOffsetY = 10.0f;
    float barOffsetX = 5.0f;
    float barPadding = 5.0f;

    // UI - power bar
    ST_Vector2D powerBarDim{ 416.0f, 32.0f };

    // UI - health
    ST_Vector2D healthSize{ 416.0f, 16.0f };

    // Overlay
    float overlayW = 350.0f;
    float overlayH = 251.0f;

    // Buttons
    float buttonW = 512.0f;
    float buttonH = 320.0f;
    float buttonOffsetX = 150.0f;
    float buttonOffsetY = 100.0f;

    // Map path
    std::string mapPath;
    std::string bgPath;
};

class ST_Scene
{
public:
    ST_Scene( const ST_GameMetadata& meta );
    ~ST_Scene() = default;


    //template<typename T, typename... Args>
    //T& addComponent( const Entity& entity, Args&&... args )
    //{
    //    return m_Registry->addComponent<T>( entity, std::forward<Args>( args )... );
    //}
    ST_Layer& createLayer();
    Camera& createCamera();

    template<typename T>
    T& addSystem()
    {
        return m_SystemManager->addSystem<T>();
    }

    template<typename... Systems>
    void registerLayer( ST_Layer& layer )
    {
        m_SystemManager->registerLayer<Systems...>( layer );
    }

    template<typename T>
    void registerEventHandler( const ST_EventManager::Handler& handler )
    {
        m_EventManager->subscribe<T>( handler );
    }

    inline Camera& getCamera() { return *m_Camera.get(); }

    friend class ST_SceneManager;

private:
    void update( float delta, SDL_Event& event );
    void render();

    ST_Entity& createPlayer(
        ST_Layer& layer
        , const ST_GameMetadata& meta
        , int playerId
        , const ST_Vector2D& pos
        , const std::string& animName
        , const std::string& animPath
        , const std::string& texturePath
    );

    void createShootingAngleUI(
        ST_Layer& layer
        , SDL_Texture* pointerTexture
        , SDL_Texture* frameTexture
        , const ST_GameMetadata& meta
        , const ST_Vector2D& playerPos
        , int id
    );

    void createPowerBar(
        ST_Layer& layer
        , SDL_Texture* gradientTexture
        , SDL_Texture* outlineTexture
        , const ST_GameMetadata& meta
        , float offsetX
        , float offsetY
        , int id
        , bool flip );

    ST_Entity& createHealthBar(
        ST_Layer& layer
        , SDL_Texture* healthTexture
        , const ST_GameMetadata& meta
        , float offsetX
        , float offsetY
        , int id
        , bool flip
    );

    ST_Entity& createButton(
        ST_Layer& layer
        , ST_Entity& parent
        , const std::string& texturePath
        , const SDL_FRect& parentRect
        , float offsetX
        , float offsetY
        , const ST_GameMetadata& meta
        , std::function<void()> onClick
    );

private:
    //std::unique_ptr<ST_Registry> m_Registry = nullptr;

    std::vector<std::unique_ptr<ST_Layer>> m_Layers{};
    std::unique_ptr<ST_SystemManager> m_SystemManager = std::make_unique<ST_SystemManager>();
    std::unique_ptr<ST_EventManager> m_EventManager = std::make_unique<ST_EventManager>();


    std::unique_ptr<Camera> m_Camera = nullptr;
};