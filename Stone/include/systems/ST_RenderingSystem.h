#pragma once

#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_TextureManager.h"

#include <memory>
#include <vector>

class ST_RenderingSystem
{
public:
    static void render( std::vector<std::unique_ptr<ST_Entity>>& entities, Camera* camera, bool debugMode )
    {
        for (auto& entity : entities) {
            if (!entity->isActive())
                continue;

            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto& transform = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                // Convert from world coordinates to screen coordinates
                // by subtracting the camera's position from the entity's position
                sprite.dest.x = transform.position.x - camera->view.x;
                sprite.dest.y = transform.position.y - camera->view.y;

                //// If the entity has an animation component, we need to update the source rect to reflect the current frame of the animation
                //if (entity->hasComponent<Animation>()) {
                //    auto& animation = entity->getComponent<Animation>();
                //    AnimationClip clip = animation.clips[animation.currentClip];
                //    sprite.src = clip.frameIndices[animation.currentFrame];
                //}

                RenderContext context{ sprite.texture, &sprite.src, &sprite.dest };
                ST_TextureManager::draw( context );
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