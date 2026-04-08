#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_HUDSystem : public ST_ISystem
{
public:
    /*
    * This system updates player's state and reset player's action flags
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        ST_Entity* A = nullptr;
        ST_Entity* B = nullptr;

        // Find players
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerTag>()) {
                if (!A)
                    A = entity.get();
                else
                    B = entity.get();
            }
        }
    }
};