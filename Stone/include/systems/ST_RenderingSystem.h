#pragma once

#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_TextureManager.h"
#include "ST_ISystem.h"

#include <memory>
#include <vector>

class ST_RenderingSystem : public ST_IRenderSystem
{
public:
    void render( std::vector<std::unique_ptr<ST_Entity>>& entities, Camera* camera, bool debugMode )
    {
        for (auto& entity : entities) {
            if (!entity->isActive())
                continue;

            if (
                entity->hasComponent<Transform>()
                && entity->hasComponent<Sprite>()
                && !entity->hasComponent<MapTile>()
                )
            {
                auto& transform = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                // Convert from world coordinates to screen coordinates
                // by subtracting the camera's position from the entity's position
                sprite.dest.x = transform.position.x - camera->view.x;
                sprite.dest.y = transform.position.y - camera->view.y;

                // If the entity has an animation component, we need to update the source rect to reflect the current frame of the animation
                if (entity->hasComponent<Animation>()) {
                    auto& animation = entity->getComponent<Animation>();
                    ST_AnimationClip clip = animation.clips[animation.currentClip];
                    sprite.src = clip.frameIndices[animation.currentFrame];
                }

                // If the entity has velocity, check its facing direction and flip the sprite accordingly
                if (entity->hasComponent<Velocity>()) {
                    int facing = entity->getComponent<Velocity>().facing;
                    sprite.flip = facing > 0 ? false : true;
                }


                RenderContext context{ sprite.texture, &sprite.src, &sprite.dest, sprite.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE };
                ST_TextureManager::draw( context );
            }

            if (debugMode) {
                renderDebug( entities );
            }
        }
    }
};