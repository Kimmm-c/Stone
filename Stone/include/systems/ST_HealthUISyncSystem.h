#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_HealthUISyncSystem : public ST_ISystem
{
public:
    /*
    * This system syncs the player's health with health UI element
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        ST_Entity* player = nullptr;

        for (auto& entity : entities) {
            if (
                entity->hasComponent<PlayerTag>()
                && entity->hasComponent<PendingHealthUIUpdateTag>()
                && entity->hasComponent<Health>()
                )
            {
                player = entity.get();
                entity->deactivate<PendingHealthUIUpdateTag>();
                break;
            }
        }

        if (!player)
            return;

        int playerId = player->getComponent<PlayerTag>().id;
        ST_Vector2D& healthRange = player->getComponent<Health>().range;
        float currentHealth = player->getComponent<Health>().current;


        // Get the health UI element associating with the player and update its sprite dimension
        for (auto& entity : entities) {
            if (entity->hasComponent<HealthUITag>()) {
                if (
                    entity->getComponent<HealthUITag>().id == playerId
                    && entity->hasComponent<Sprite>()
                    )
                {
                    Sprite& healthUI = entity->getComponent<Sprite>();
                    float percentage = std::clamp( currentHealth / healthRange.y, 0.0f, 1.0f );
                    healthUI.src.w = healthUI.dest.w = percentage * healthUI.fixedDimension.x;
                }
            }
        }
    }
};