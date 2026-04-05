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

        ST_Entity* playerA = nullptr;
        ST_Entity* playerB = nullptr;

        assignPlayers( entities, playerA, playerB );

        if (!playerA && !playerB)
            return;

        if (playerA)
            updatePlayerShootingAngleUI( entities, playerA );

        if (playerB)
            updatePlayerShootingAngleUI( entities, playerB );

    }

private:
    void assignPlayers( const std::vector<std::unique_ptr<ST_Entity>>& entities, ST_Entity*& playerA, ST_Entity*& playerB )
    {
        for (auto& entity : entities) {
            if (
                entity->hasComponent<PlayerTag>()
                && entity->hasComponent<Transform>()
                && entity->hasComponent<Projectile>()
                && entity->hasComponent<Velocity>()
                && entity->hasComponent<Sprite>()
                )
            {
                int id = entity->getComponent<PlayerTag>().id;

                if (id == 0)
                    playerA = entity.get();
                else if (id == 1)
                    playerB = entity.get();
            }
        }
    }

    void updatePlayerShootingAngleUI( const std::vector<std::unique_ptr<ST_Entity>>& entities, ST_Entity* player )
    {
        // extract transform and projectile component
        Transform& playerTransform = player->getComponent<Transform>();
        float facingDir = player->getComponent<Velocity>().facing;
        Projectile& playerProjectile = player->getComponent<Projectile>();
        int playerId = player->getComponent<PlayerTag>().id;

        // center UI to player using player's sprite dimension
        float playerCenterX = playerTransform.position.x;
        float playerCenterY = playerTransform.position.y;

        auto& sprite = player->getComponent<Sprite>();
        playerCenterX += sprite.dest.w * 0.5f;
        playerCenterY += sprite.dest.h * 0.5f;

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

                    // position UI to player's center
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

                    float frameHalfW = frameSprite.dest.w * 0.5f;
                    float frameHalfH = frameSprite.dest.h * 0.5f;
                    frameTransform.position.x = playerCenterX - frameHalfW;
                    frameTransform.position.y = playerCenterY - frameHalfH;

                    frameSprite.flip = (facingDir < 0) ? true : false;
                }
            }
        }
    }
};