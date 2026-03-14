#if 0
#include "Scene.h"
#include "Registry.h"
#include "SystemScheduler.h"

Scene::Scene()
    : m_Registry( std::make_unique<Registry>() )
    , m_SystemScheduler( std::make_unique<SystemScheduler>() )
{}

Entity& Scene::createEntity()
{

}

void Scene::update( float delta, const SDL_Event& event )
{

}

void Scene::render( const std::unique_ptr<Renderer>& renderer )
{

}

#endif