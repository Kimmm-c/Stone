#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_CameraSystem : public ST_ISystem
{
public:
    /*
    * Camera system:
    * 1. Focuses on active player's position.x during Aiming state and
    * 2. Focuses on projectile's position.x during Resolving state
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();
        // Get camera
        Camera& camera = context.camera;

        // There's supposed to be only 1 target per frame (either active player or projectile)
        ST_Entity* target = nullptr;

        for (auto& entity : entities) {
            // Get active player
            if (entity->hasComponent<ActivePlayerTag>() && entity->hasComponent<Transform>()) {
                target = entity.get();
                break;
            }
        }

        // When there is no active player, get the projectileTag
        if (!target) {
            for (auto& entity : entities) {
                if (entity->hasComponent<ProjectileTag>() && entity->hasComponent<Transform>()) {
                    target = entity.get();
                    break;
                }
            }
        }

        if (!target)
            return;

        // Move camera following the target's position.x
        // Center the camera on the player
        Transform& targetTransform = target->getComponent<Transform>();
        camera.view.x = targetTransform.position.x - camera.view.w / 2;
        camera.view.y = targetTransform.position.y - camera.view.h / 2;

        // Clamp the camera to the world bounds
        if (camera.view.x < 0) {
            camera.view.x = 0;
        }
        else if (camera.view.x + camera.view.w > camera.worldWidth) {
            camera.view.x = camera.worldWidth - camera.view.w;
        }
        if (camera.view.y < 0) {
            camera.view.y = 0;
        }
        else if (camera.view.y + camera.view.h > camera.worldHeight) {
            camera.view.y = camera.worldHeight - camera.view.h;
        }
    }
};