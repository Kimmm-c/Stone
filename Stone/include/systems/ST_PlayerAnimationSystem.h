#pragma once

#include <vector>
#include <memory>

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_PlayerAnimationSystem : public ST_ISystem
{
public:
    /*
    * This system decide player animation clip based on player's state
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (
                entity->hasComponent<PlayerTag>()
                && entity->hasComponent<PlayerStateComponent>()
                && entity->hasComponent<Animation>()
                )
            {
                std::string newClip;
                Animation& anim = entity->getComponent<Animation>();
                PlayerStateComponent& state = entity->getComponent<PlayerStateComponent>();

                switch (state.state) {
                case PlayerState::Falling:
                    newClip = "falldown";
                    break;
                case PlayerState::Hurt:
                    newClip = "hurt";
                    break;
                case PlayerState::Walk:
                    newClip = "walk";
                    break;
                case PlayerState::Throwing:
                    newClip = "throw";
                    break;
                default:
                    newClip = "idle";
                    break;
                }

                // Check if the animation has switched
                // if the chosen clip is different from the new clip, reset time and frame index
                if (anim.currentClip != newClip) {
                    anim.currentClip = newClip;
                    anim.currentFrame = 0; // reset to the first frame of the new clip
                    anim.time = 0.0f;      // reset the time accumulator
                };
            }
        }
    }
};