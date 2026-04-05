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
            if (entity->hasComponent<Animation>()) {
                auto& animation = entity->getComponent<Animation>();

                // Playback system: advance frame if accumulated time exceeds frame speed
                float animationFrameSpeed = animation.speed;
                ST_AnimationClip clip = animation.clips[animation.currentClip];
                animation.time += context.delta;
                animation.isFinished = false;

                if (animation.time >= animationFrameSpeed) {
                    animation.time -= animationFrameSpeed;

                    std::size_t totalAnimationFrames = clip.frameIndices.size();
                    //animation.currentFrame = (animation.currentFrame + 1) % totalAnimationFrames; // advance to the next frame, loop back to the start if we reach the end

                    // loop back to beginning frame if animation is looping, otherwise, mark as finished
                    if (animation.currentFrame == totalAnimationFrames - 1)
                    {
                        if (clip.loop)
                            animation.currentFrame = 0;
                        else
                            animation.isFinished = true;
                    }
                    else
                        animation.currentFrame++;
                }
            }
        }
    }
};