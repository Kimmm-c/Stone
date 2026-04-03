#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"

class ST_ShootingAngleUISyncSystem : public ST_ISystem
{
public:
    /*
    * Ths system updates the shooting angle UI based on player's shooting angle and facing direction
    */
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        ST_Entity* player = nullptr;

        // Get active player
        for (auto& entity : entities) {
            if (
                entity->hasComponent<ActivePlayerTag>()
                && entity->hasComponent<Transform>()
                && entity->hasComponent<Projectile>()
                && entity->hasComponent<Velocity>()
                )
            {
                player = entity.get();
                break;
            }
        }

        if (!player)
            return;


        // extract transform and projectile component
        Transform& playerTransform = player->getComponent<Transform>();
        float facingDir = player->getComponent<Velocity>().facing;
        Projectile& playerProjectile = player->getComponent<Projectile>();
        int playerId = player->getComponent<PlayerTag>().id;

        // TODO: replace hardcoded dimension
        float playerCenterX = playerTransform.position.x + 32.0f;
        float playerCenterY = playerTransform.position.y + 32.0f;

        for (auto& entity : entities) {
            // Update the angle pointer associating with the player using player's transform + shooting angle + facing direction
            if (
                entity->hasComponent<AnglePointerUI>()
                && entity->hasComponent<Transform>()
                )
            {
                AnglePointerUI& pointer = entity->getComponent<AnglePointerUI>();

                if (pointer.id == playerId) {
                    // Update pointer's position
                    Transform& pointerTransform = entity->getComponent<Transform>();
                    float angleDeg = playerProjectile.angle;
                    float angleRad = angleDeg * 3.14159265f / 180.0f;

                    float offsetX = std::cos( angleRad ) * pointer.distFromPlayer * facingDir;
                    float offsetY = -std::sin( angleRad ) * pointer.distFromPlayer;

                    // apply offset to pointer
                    pointerTransform.position.x = playerCenterX + offsetX;
                    pointerTransform.position.y = playerCenterY + offsetY;
                }
            }

            // Flip the angle frame associating with the player based on player's facing direction
            if (
                entity->hasComponent<AngleFrameUITag>()
                && entity->hasComponent<Sprite>()
                )
            {
                AngleFrameUITag& frame = entity->getComponent<AngleFrameUITag>();


                if (frame.id == playerId) {
                    Transform& frameTransform = entity->getComponent<Transform>();
                    Sprite& frameSprite = entity->getComponent<Sprite>();

                    // TODO: replace hardcoded dimension
                    frameTransform.position.x = playerCenterX - 64.0f;  // frame is 128x128 -> offset by 64 to center on player
                    frameTransform.position.y = playerCenterY - 64.0f;

                    frameSprite.flip = (facingDir < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                }
            }
        }
    }
};