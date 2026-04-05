#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"
#include "ST_Collision.h"
#include "ST_BaseEvent.h"
#include "ST_EventManager.h"

class ST_MouseInputSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& event = context.event;

        if (
            event.type != SDL_EVENT_MOUSE_MOTION
            && event.type != SDL_EVENT_MOUSE_BUTTON_DOWN
            && event.type != SDL_EVENT_MOUSE_BUTTON_UP
            )
            return;

        auto& entities = layer.getEntities();

        float mx, my;
        SDL_GetMouseState( &mx, &my );

        for (auto& entity : entities) {
            if (entity->hasComponent<Clickable>() && entity->hasComponent<Collider>()) {
                Clickable& clickable = entity->getComponent<Clickable>();
                Collider& collider = entity->getComponent<Collider>();

                if (!collider.isEnabled)
                    continue;

                bool inside = ST_Collision::pointIsInside( ST_Vector2D( mx, my ), collider );

                // Hover
                if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    if (!inside && clickable.pressed) {
                        // cancel event
                        context.eventManager.emit<ST_MouseInteractionEvent>( ST_MouseInteractionEvent( entity.get(), MouseInteractionState::Cancel ) );
                    }
                }

                // Press
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (inside) {
                            clickable.pressed = true;

                            // pressed event
                            context.eventManager.emit<ST_MouseInteractionEvent>( ST_MouseInteractionEvent( entity.get(), MouseInteractionState::Pressed ) );
                        }

                    }
                }

                // Press
                if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (inside) {
                            clickable.pressed = false;

                            // released event
                            context.eventManager.emit<ST_MouseInteractionEvent>( ST_MouseInteractionEvent( entity.get(), MouseInteractionState::Released ) );
                        }

                    }
                }
            }
        }
    }
};