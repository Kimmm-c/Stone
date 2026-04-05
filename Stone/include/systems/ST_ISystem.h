#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "ST_Entity.h"

class ST_Layer;
class ST_EventManager;

struct ST_SystemContext
{
    float delta;
    SDL_Event& event;
    ST_EventManager& eventManager;

    Camera& camera;
};

class ST_ISystem
{
public:
    virtual void update( ST_Layer& layer, const ST_SystemContext& context ) = 0;
};



class ST_IRenderSystem
{
public:
    virtual void render( std::vector<std::unique_ptr<ST_Entity>>& entities, Camera* camera, bool debugMode ) = 0;

protected:
    void renderDebug( std::vector<std::unique_ptr<ST_Entity>>& entities )
    {
        for (auto& entity : entities) {
            if (entity->hasComponent<Collider>() && entity->hasComponent<Transform>()) {
                Collider collider = entity->getComponent<Collider>();

                if (!m_DebugTexture) {
                    m_DebugTexture = ST_TextureManager::load( std::string( ASSET_PATH ) + "spritesheet.png" );
                }

                SDL_FRect colliderSrc{ 0, 32, 32, 32 };
                SDL_FRect colliderDest{ collider.rect.x, collider.rect.y, collider.rect.w, collider.rect.h };

                // Reduce the opacity of the debug overlay
                SDL_SetTextureAlphaMod( m_DebugTexture, 100 );
                SDL_SetTextureBlendMode( m_DebugTexture, SDL_BLENDMODE_BLEND );

                ST_TextureManager::draw( { m_DebugTexture, &colliderSrc, &colliderDest } );
            }
        }
    }

private:
    SDL_Texture* m_DebugTexture = nullptr;
};