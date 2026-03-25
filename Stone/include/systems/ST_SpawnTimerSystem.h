#pragma once

#include <vector>
#include <memory>

#include "ST_ISystem.h"
#include "ST_Entity.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_SpawnTimerSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context )
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<TimedSpawner>()) {
                auto& spawner = entity->getComponent<TimedSpawner>();

                spawner.timer -= context.delta;

                if (spawner.timer <= 0) {
                    spawner.timer = spawner.spawnInterval; // eg: 2s
                    spawner.spawnCallback(); // create entity inside here
                }
            }
        }
    }
};