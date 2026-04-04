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
            if (entity->hasComponent<PendingProjectileTag>()) {
                Projectile& projectile = entity->getComponent<Projectile>();
                Transform& projTransform = entity->getComponent<Transform>();

                // Calculate the velocity of the projectile entity
                projectile.angle = std::clamp( projectile.angle, 0, 89 );
                float angleRad = (float)projectile.angle * 3.14159265f / 180.0f;

                float x = std::cos( angleRad );
                float y = -std::sin( angleRad );

                Velocity& projVelocity = entity->getComponent<Velocity>();
                projVelocity.direction = ST_Vector2D( x * projVelocity.direction.x, y ).normalize();
                projVelocity.speed = projectile.currentForce;

                if (!m_ProjectileTexture)
                    m_ProjectileTexture = ST_TextureManager::load( std::string( ASSET_PATH ) + "stone.PNG" );


                // TODO: replace hardcoded
                SDL_FRect src{ 0, 0, 512, 512 };
                SDL_FRect dest{ projTransform.position.x, projTransform.position.y, 32, 32 };

                entity->addComponent<Sprite>( m_ProjectileTexture, src, dest );
                Collider& collision = entity->addComponent<Collider>( "destructiveProjectile" );

                collision.rect.w = dest.w;
                collision.rect.h = dest.h;

                entity->addComponent<DestructiveProjectileTag>();
                entity->deactivate<PendingProjectileTag>();
                entity->addComponent<ProjectileTag>();
            }
        }
    }

private:
    SDL_Texture* m_ProjectileTexture = nullptr;
};