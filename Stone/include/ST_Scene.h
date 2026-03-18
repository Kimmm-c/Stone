#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "ST_Layer.h"
#include "ST_ISystem.h"

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


    friend class ST_SceneManager;

private:
    void update( float delta, const SDL_Event& event );
    void render();

private:
    //std::unique_ptr<ST_Registry> m_Registry = nullptr;
    //std::unique_ptr<SystemScheduler> m_SystemScheduler = nullptr;

    std::vector<std::unique_ptr<ST_Layer>> m_Layers{};
    std::vector<std::unique_ptr<ST_ISystem>> m_Systems{};
};