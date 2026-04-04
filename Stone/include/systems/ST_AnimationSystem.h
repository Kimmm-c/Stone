#pragma once

#include <vector>
#include <memory>

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_AnimationSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (const auto& entity : entities) {
            if (entity->hasComponent<Animation>() && entity->hasComponent<Velocity>()) {
                auto& animation = entity->getComponent<Animation>();
                auto& velocity = entity->getComponent<Velocity>();

                // State system: Deciding which clip to use
                std::string newClip;

                if (velocity.direction.x > 0.0f) {
                    newClip = "walk";
                }
                else if (velocity.direction.x < 0.0f) {
                    newClip = "walk";
                }
                else if (velocity.direction.y > 0.0f) {
                    newClip = "falldown";
                }
                //else if (velocity.direction.y < 0.0f) {
                //    newClip = "walk_up";
                //}
                else {
                    newClip = "idle";
                }

                // Check if the animation has switched
                // if the chosen clip is different from the new clip, reset time and frame index
                if (animation.currentClip != newClip) {
                    animation.currentClip = newClip;
                    animation.currentFrame = 0; // reset to the first frame of the new clip
                    animation.time = 0.0f;      // reset the time accumulator
                };

                // Playback system: advance frame if accumulated time exceeds frame speed
                float animationFrameSpeed = animation.speed;
                ST_AnimationClip clip = animation.clips[animation.currentClip];
                animation.time += context.delta;

                if (animation.time >= animationFrameSpeed) {
                    animation.time -= animationFrameSpeed;

                    std::size_t totalAnimationFrames = clip.frameIndices.size();
                    animation.currentFrame = (animation.currentFrame + 1) % totalAnimationFrames; // advance to the next frame, loop back to the start if we reach the end
                }
            }
        }
    }
};