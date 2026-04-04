#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_ScreenUISystem : public ST_ISystem
{
public:
    /*
    * This system offsets UI by camera position so that UI always stay at fixed position in the viewport
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();
        const Camera& camera = context.camera;

        for (auto& entity : entities)
        {
            if (!entity->hasComponent<Transform>() || !entity->hasComponent<ScreenSpaceUI>())
                continue;

            Transform& transform = entity->getComponent<Transform>();
            const auto& ui = entity->getComponent<ScreenSpaceUI>();

            // recompute from base position every frame
            transform.position.x = ui.screenPosition.x + camera.view.x;
            transform.position.y = ui.screenPosition.y + camera.view.y;
        }
    }
};