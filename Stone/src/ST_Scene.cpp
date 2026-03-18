#include "ST_Scene.h"

ST_Layer& ST_Scene::createLayer()
{
    m_Layers.emplace_back( std::make_unique<ST_Layer>() );

    return *m_Layers.back();
}

void ST_Scene::update( float delta, const SDL_Event& event )
{
    for (auto& layer : m_Layers)
        layer->update( delta, event );
}

void ST_Scene::render()
{
    for (auto& layer : m_Layers)
        layer->render();
}