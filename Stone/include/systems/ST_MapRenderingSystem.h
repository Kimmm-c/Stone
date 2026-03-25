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

                if (debugMode) {
                    ST_TextureManager::draw( { sprite.texture, &sprite.src, &sprite.dest } );

                    // Create an overlay and draw it on top of the sprite
                    ST_TextureManager::draw( {
                        ST_TextureManager::load(std::string(ASSET_PATH) + "spritesheet.png")
                                             })
                }
                else {
                    ST_TextureManager::draw( { sprite.texture, &sprite.src, &sprite.dest } );
                }
            }
        }
    }
};