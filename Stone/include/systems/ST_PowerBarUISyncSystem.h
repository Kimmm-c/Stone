#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_PowerBarUISyncSystem : public ST_ISystem
{
public:
    /*
    * This system syncs the shooting force of projectiles with the power bar UI
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        ST_Entity* player = nullptr;

        // Get the player entity and extract the projectile component
        for (auto& entity : entities) {
            if (entity->hasComponent<ActivePlayerTag>() && entity->hasComponent<Projectile>()) {
                player = entity.get();
                break;
            }
        }

        if (!player)
            return;

        // map the projectile's current force to percentage of the power bar
        Projectile projectile = player->getComponent<Projectile>();
        Force forceRange = projectile.force;
        float percentage = static_cast<float>(projectile.currentForce - forceRange.min) / static_cast<float>(forceRange.max - forceRange.min);

        // Get the power bar associating with the player
        for (auto& entity : entities) {
            if (entity->hasComponent<PowerBarTag>()) {
                if (entity->getComponent<PowerBarTag>().id == player->getComponent<PlayerTag>().id
                     && entity->hasComponent<Sprite>()) {
                    // use the percentage to update the source and dest width of the power bar sprite
                    Sprite& powerRep = entity->getComponent<Sprite>();
                    powerRep.src.w = powerRep.dest.w = percentage * powerRep.fixedDimension.x;
                }
            }
        }


    }
};