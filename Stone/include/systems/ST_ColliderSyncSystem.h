#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_ColliderSyncSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities)
        {
            if (!entity->isActive()) continue;

            if (entity->hasComponent<Transform>() &&
                 entity->hasComponent<Collider>())
            {
                auto& transform = entity->getComponent<Transform>();
                auto& collider = entity->getComponent<Collider>();

                // keep collider in sync
                collider.rect.x = transform.position.x;
                collider.rect.y = transform.position.y;
            }
        }
    }
};