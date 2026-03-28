#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"
#include "ST_BaseEvent.h"
#include "ST_EventManager.h"

class ST_KeyboardInputSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        // emit the user input
        auto& entities = layer.getEntities();

        if (context.event.type == SDL_EVENT_KEY_DOWN || context.event.type == SDL_EVENT_KEY_UP) {
            for (auto& entity : entities) {
                if (!entity->isActive()) continue;

                if (entity->hasComponent<PlayerTag>()) {
                    context.eventManager.emit<ST_PlayerActionEvent>( ST_PlayerActionEvent( entity.get(), layer, context ) );
                }

                //if (entity->hasComponent<ProjectileAngleTag>()) {
                //    context.eventManager.emit<ST_ProjectileAngleEvent>( ST_ProjectileAngleEvent( entity.get(), context.event ) );
                //}

                //if (entity->hasComponent<ProjectileChargeTag>()) {
                //    context.eventManager.emit<ST_ProjectileChargeEvent>( ST_ProjectileChargeEvent( entity.get(), context.event ) );
                //}
            }
        }
    }
};