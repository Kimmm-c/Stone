#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_MovementSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
                auto& transform = entity->getComponent<Transform>();
                auto& velocityComponent = entity->getComponent<Velocity>();

                // set old position
                transform.oldPosition = transform.position;

                // Calculate the velocity
                ST_Vector2D velocity = velocityComponent.direction.normalize() * velocityComponent.speed;

                // Calculate the position
                transform.position += velocity * context.delta;
            }
        }
    }
};