#pragma once

#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_TextureManager.h"

#include <memory>
#include <vector>

class ST_MapRenderingSystem
{
public:
    static void render( std::vector<std::unique_ptr<ST_Entity>>& entities, Camera* camera, bool debugMode )
    {
        for (auto& entity : entities) {
            if (!entity->isActive())
                continue;

            if (entity->hasComponent<MapTile>() && entity->hasComponent<Sprite>()) {
                auto& sprite = entity->getComponent<Sprite>();
                auto& mapTile = entity->getComponent<MapTile>();

                float worldX = static_cast<float>(mapTile.col) * sprite.dest.w;
                float worldY = static_cast<float>(mapTile.row) * sprite.dest.h;

                sprite.dest.x = std::round( worldX - camera->view.x );
                sprite.dest.y = std::round( worldY - camera->view.y );

                ST_TextureManager::draw( { sprite.texture, &sprite.src, &sprite.dest } );
            }

            if (debugMode) {
                if (entity->hasComponent<Collider>() && entity->hasComponent<Transform>()) {
                    Collider collider = entity->getComponent<Collider>();

                    SDL_Texture* colliderTexture = ST_TextureManager::load( std::string( ASSET_PATH ) + "spritesheet.png" );
                    SDL_FRect colliderSrc{ 0, 32, 32, 32 };
                    SDL_FRect colliderDest{ collider.rect.x, collider.rect.y, collider.rect.w, collider.rect.h };

                    // Reduce the opacity of the debug overlay
                    SDL_SetTextureAlphaMod( colliderTexture, 100 );
                    SDL_SetTextureBlendMode( colliderTexture, SDL_BLENDMODE_BLEND );

                    ST_TextureManager::draw( { colliderTexture, &colliderSrc, &colliderDest } );
                }
            }
        }
    }
};