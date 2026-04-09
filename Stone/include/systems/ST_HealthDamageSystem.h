#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"
#include "ST_MathHelper.h"

class ST_HealthDamageSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<DamageAccumulator>() && !entity->hasComponent<DamageAppliedTag>()) {
                auto& acc = entity->getComponent<DamageAccumulator>();

                if (acc.targetPlayer && acc.targetPlayer->hasComponent<Health>()) {

                    // TODO: replace hardcoded ranges
                    ST_Vector2D overlappingRange{ 0.0f, 32.0f * 32.0f }; // 32x32 is the projectile area
                    ST_Vector2D healthRange{ 0.0f, 5000.0f };

                    float overlap = std::min( acc.maxOverlap, overlappingRange.y );
                    float damage = ST_MathHelper::map( overlap, overlappingRange, healthRange );

                    acc.targetPlayer->getComponent<Health>().current -= damage;
                    acc.targetPlayer->addComponent<PendingHealthUIUpdateTag>();
                }

                entity->addComponent<DamageAppliedTag>();
            }
        }
    }
};