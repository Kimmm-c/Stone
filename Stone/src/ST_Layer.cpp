#include "ST_Layer.h"

ST_Entity& ST_Layer::createEntity()
{
    m_Entities.emplace_back( std::make_unique<ST_Entity>() );

    return *m_Entities.back();
}

void ST_Layer::update( float delta, const SDL_Event& event )
{
    cleanup();
}

void ST_Layer::render( Camera* camera )
{
    if (camera) {
        m_MapRenderingSystem->render( m_Entities, camera, false );
        m_RenderingSystem->render( m_Entities, camera, true );
    }

    m_UIRenderingSystem->render( m_Entities, camera, false ); // UI elements are rendered regardless
}

void ST_Layer::cleanup()
{
    // erase if entity is not active
    m_Entities.erase( std::remove_if( m_Entities.begin(), m_Entities.end(),
                                      []( const std::unique_ptr<ST_Entity>& entity ) {

                                          if (entity->hasComponent<PendingDestroy>())
                                              entity->destroy();

                                          return !entity->isActive();
                                      } ),
                      m_Entities.end() );
}