#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_PhysicsSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities)
        {
            if (!entity->isActive()) continue;

            if (entity->hasComponent<Transform>() &&
                 entity->hasComponent<Velocity>())
            {
                auto& t = entity->getComponent<Transform>();
                auto& v = entity->getComponent<Velocity>();

                v.direction.y += 3.0f * context.delta;

                t.oldPosition = t.position;

                t.position.x += v.direction.x * v.speed * context.delta;
                t.position.y += v.direction.y * v.speed * context.delta;
            }
        }
    }
};