#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_ProjectileDestructionSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();
        Camera& camera = context.camera;

        for (auto& entity : entities) {
            if (entity->hasComponent<ProjectileTag>() && entity->hasComponent<Transform>()) {
                auto& transform = entity->getComponent<Transform>();
                const float x = transform.position.x;
                const float y = transform.position.y;

                if (x < camera.view.x || x > camera.view.w || y < camera.view.y || y > camera.view.h)
                {
                    entity->destroy();
                }
            }
        }
    }
};