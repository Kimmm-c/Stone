#pragma once

#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_TextureManager.h"

#include <memory>
#include <vector>

class ST_RenderingSystem
{
public:
    static void render( std::vector<std::unique_ptr<ST_Entity>>& entities )
    {
        // Find the camera
        ST_Entity* cameraEntity = nullptr;

        for (const auto& entity : entities) {
            if (entity->hasComponent<Camera>()) {
                cameraEntity = entity.get();
                break;
            }
        };

        if (!cameraEntity) {
            return;
        }

        Camera& camera = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto& transform = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                // Convert from world coordinates to screen coordinates
                // by subtracting the camera's position from the entity's position
                sprite.dest.x = transform.position.x - camera.view.x;
                sprite.dest.y = transform.position.y - camera.view.y;

                //// If the entity has an animation component, we need to update the source rect to reflect the current frame of the animation
                //if (entity->hasComponent<Animation>()) {
                //    auto& animation = entity->getComponent<Animation>();
                //    AnimationClip clip = animation.clips[animation.currentClip];
                //    sprite.src = clip.frameIndices[animation.currentFrame];
                //}

                ST_TextureManager::draw( sprite.texture, sprite.src, sprite.dest );
            }
        }
    }
};