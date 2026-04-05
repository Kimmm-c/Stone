#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_PlayerStateSystem : public ST_ISystem
{
public:
    /*
    * This system updates player's state and reset player's action flags
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities)
        {
            if (
                !entity->hasComponent<PlayerStateComponent>()
                || !entity->hasComponent<Velocity>()
                || !entity->hasComponent<Animation>()
                || !entity->hasComponent<PlayerActionFlags>()
                )
                continue;

            auto& state = entity->getComponent<PlayerStateComponent>();
            auto& velocity = entity->getComponent<Velocity>();
            auto& animation = entity->getComponent<Animation>();
            auto& flags = entity->getComponent<PlayerActionFlags>();

            // Blocking states for one-time (non-looping) actions
            if (state.state == PlayerState::Throwing || state.state == PlayerState::Hurt)
            {
                if (!animation.isFinished)
                    continue;
            }


            if (flags.isHurt)
            {
                state.state = PlayerState::Hurt;
            }
            else if (flags.isThrowing)
            {
                state.state = PlayerState::Throwing;
            }
            else if (velocity.direction.y > 0)
            {
                state.state = PlayerState::Falling;
            }
            else if (velocity.direction.x != 0)
            {
                state.state = PlayerState::Walk;
            }
            else
            {
                state.state = PlayerState::Idle;
            }

            flags.isThrowing = false;
            flags.isHurt = false;
        }
    }
};