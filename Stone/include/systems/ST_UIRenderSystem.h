#pragma once

#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_TextureManager.h"
#include "ST_ISystem.h"
#include "ST_RenderHelper.h"

#include <memory>
#include <vector>

class ST_UIRenderSystem : public ST_IRenderSystem
{
public:
    void render( std::vector<std::unique_ptr<ST_Entity>>& entities, Camera* camera, bool debugMode )
    {
        for (auto& entity : entities) {
            if (!entity->isActive())
                continue;

            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                Transform& transform = entity->getComponent<Transform>();
                Sprite& sprite = entity->getComponent<Sprite>();

                // Filter out anything that is not UI
                if (sprite.renderLayer != RenderLayer::UI)
                    continue;

                // no converting from world space to screen space
                sprite.dest.x = transform.position.x;
                sprite.dest.y = transform.position.y;

                // TODO: Separate sprite update for animation to another system and remove the
                // below block of code from render systems
                // If the entity has an animation component, we need to update the source rect to reflect the current frame of the animation
                if (entity->hasComponent<Animation>()) {
                    auto& animation = entity->getComponent<Animation>();
                    ST_AnimationClip clip = animation.clips[animation.currentClip];
                    sprite.src = clip.frameIndices[animation.currentFrame];
                }

                if (sprite.isVisible) {
                    SDL_FRect scaledDest = ST_RenderHelper::getScaledDest( sprite.dest, transform.scale );
                    ST_TextureManager::draw( { sprite.texture, &sprite.src, &scaledDest } );
                }
            }
        }
    }
};