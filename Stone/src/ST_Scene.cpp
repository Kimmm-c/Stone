#include "ST_Scene.h"
#include "ST_SystemManager.h"

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