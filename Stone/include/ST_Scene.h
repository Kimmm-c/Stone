#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "ST_Layer.h"
#include "ST_ISystem.h"
#include "ST_SystemManager.h"

class ST_Renderer;

class ST_Scene
{
public:
    ST_Scene() = default;
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

    friend class ST_SceneManager;

private:
    void update( float delta, SDL_Event& event );
    void render();

private:
    //std::unique_ptr<ST_Registry> m_Registry = nullptr;
    //std::unique_ptr<SystemScheduler> m_SystemScheduler = nullptr;

    std::vector<std::unique_ptr<ST_Layer>> m_Layers{};
    std::unique_ptr<ST_SystemManager> m_SystemManager = std::make_unique<ST_SystemManager>();


    std::unique_ptr<Camera> m_Camera = nullptr;
};