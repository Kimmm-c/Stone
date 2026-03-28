#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"
#include "ST_Entity.h"
#include "ST_Component.h"

#include <cmath>
#include <algorithm>

class ST_ProjectileSpawnSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<ProjectileTag>()) {
                Projectile& projectile = entity->getComponent<Projectile>();
                Transform& projTransform = entity->getComponent<Transform>();

                // Calculate the velocity of the projectile entity
                projectile.angle = std::clamp( projectile.angle, 0, 89 );
                float angleRad = (float)projectile.angle * 3.14159265f / 180.0f;

                float x = std::cos( angleRad );
                float y = -std::sin( angleRad );

                Velocity& projVelocity = entity->addComponent<Velocity>();
                projVelocity.direction = ST_Vector2D( x, y ).normalize();
                projVelocity.speed = projectile.currentForce;

                SDL_Texture* texture = ST_TextureManager::load( std::string( ASSET_PATH ) + "spritesheet.png" );
                SDL_FRect src{ 0, 0, 32, 32 };
                SDL_FRect dest{ projTransform.position.x, projTransform.position.y, 32, 32 };
                entity->addComponent<Sprite>( texture, src, dest );

                Collider& collision = entity->addComponent<Collider>( "destructiveProjectile" );

                collision.rect.w = dest.w;
                collision.rect.h = dest.h;

                entity->addComponent<DestructiveProjectileTag>();
            }
        }
    }
};