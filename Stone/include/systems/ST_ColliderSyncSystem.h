#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_ColliderSyncSystem : public ST_ISystem
{
public:
    /*
    * This system centers collider to player using player's sprite dimension
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities)
        {
            if (!entity->isActive()) continue;

            if (
                entity->hasComponent<Transform>()
                && entity->hasComponent<Collider>()
                && entity->hasComponent<Sprite>()
                )
            {
                auto& transform = entity->getComponent<Transform>();
                auto& collider = entity->getComponent<Collider>();
                auto& sprite = entity->getComponent<Sprite>();

                // center the collider to player using sprite's dimension
                float centerX = transform.position.x;
                float centerY = transform.position.y;

                centerX += sprite.dest.w * 0.5f;
                centerY += sprite.dest.h * 0.5f;

                // align collider center to player center
                collider.rect.x = centerX - collider.rect.w * 0.5f;
                collider.rect.y = centerY - collider.rect.h * 0.5f;
            }
        }
    }
};