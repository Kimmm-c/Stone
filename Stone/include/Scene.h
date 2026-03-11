#pragma once

#include <memory>
#include <SDL3/SDL.h>

class Registry;
class SystemScheduler;
class Entity;
class Renderer;

class Scene
{
public:
    Scene();
    ~Scene() = default;

    Entity& createEntity();

    template<typename T, typename... Args>
    T& addComponent( const Entity& entity, Args&&... args )
    {
        return m_Registry->addComponent<T>( entity, args );
    }


    friend class SceneManager;

private:
    void update( float delta, const SDL_Event& event );
    void render( const std::unique_ptr<Renderer>& renderer );

private:
    std::unique_ptr<Registry> m_Registry = nullptr;
    std::unique_ptr<SystemScheduler> m_SystemScheduler = nullptr;
};