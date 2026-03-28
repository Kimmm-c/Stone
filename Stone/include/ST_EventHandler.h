#pragma once

#include "ST_BaseEvent.h"
#include "ST_Component.h"
#include "ST_Entity.h"

#include <algorithm>

inline void collisionHandler( const ST_BaseEvent& event )
{
    const auto& collision = static_cast<const ST_CollisionEvent&>(event);

    ST_Entity* A = collision.entityA;
    ST_Entity* B = collision.entityB;

    Collider& colliderA = A->getComponent<Collider>();
    Collider& colliderB = B->getComponent<Collider>();

    if (colliderA.tag != "tile" && colliderB.tag == "tile") {
        Transform& transform = collision.entityA->getComponent<Transform>();
        Velocity& velocity = collision.entityA->getComponent<Velocity>();

        transform.position = transform.oldPosition;

        // Reset velocity so player doesn't overshoot platform due to large velocity
        if (velocity.direction.y > 0) {
            velocity.direction.y = 0;
        }
    }
    else if (colliderA.tag == "tile" && colliderB.tag != "tile") {
        Transform& transform = collision.entityB->getComponent<Transform>();
        Velocity& velocity = collision.entityB->getComponent<Velocity>();

        transform.position = transform.oldPosition;

        // Reset velocity so player doesn't overshoot platform due to large velocity
        if (velocity.direction.y > 0) {
            velocity.direction.y = 0;
        }
    }

    // Collision between destructive projectile and tile
    if (colliderA.tag == "destructiveProjectile" && colliderB.tag == "tile") {
        A->addComponent<PendingDestroy>();
        B->addComponent<PendingDestroy>();
    }
    else if (colliderA.tag == "tile" && colliderB.tag == "destructiveProjectile") {
        A->addComponent<PendingDestroy>();
        B->addComponent<PendingDestroy>();
    }
}

/*
* Player 1                              |Player 2
* Movement control: A/D                 |Movement control: Left/Right arrow
* Shooting angle control: W/S           |Shooting angle control: Up/Down arrow
* Shooting type control: 1, 2, 3        |Shooting type control: ',', '.', '/'
* Shooting power control: Left shift    |Shooting power control: Right shift
* Launch shot: Release left shift       |Launch shot: Release right shift
*/
inline void playerActionHandler( const ST_BaseEvent& event )
{
    const auto& playerEvent = static_cast<const ST_PlayerActionEvent&>(event);
    ST_Entity* entity = playerEvent.entity;

    if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
        auto& transform = entity->getComponent<Transform>();
        auto& velocity = entity->getComponent<Velocity>();

        // Move the player horizontally OR update projectile's properties when key is pressed
        if (playerEvent.context.event.type == SDL_EVENT_KEY_DOWN) {
            // If KeyCode is A/LeftArrow, move the entity to the left
            if (playerEvent.context.event.key.key == SDLK_A || playerEvent.context.event.key.key == SDLK_LEFT) {
                velocity.direction.x = -1.0f;
            }
            // If KeyCode is D/RightArrow, move the entity to the right
            else if (playerEvent.context.event.key.key == SDLK_D || playerEvent.context.event.key.key == SDLK_RIGHT) {
                velocity.direction.x = 1.0f;
            }

            if (entity->hasComponent<Projectile>()) {
                auto& projectile = entity->getComponent<Projectile>();

                // Input: W/UpArrow -> Increase projectile's shooting angle
                if (playerEvent.context.event.key.key == SDLK_W || playerEvent.context.event.key.key == SDLK_UP) {
                    projectile.angle = std::clamp( projectile.angle + 1, 0, 89 );
                }
                // Input: S/DownArrow -> Decrease projectile's shooting angle
                else if (playerEvent.context.event.key.key == SDLK_S || playerEvent.context.event.key.key == SDLK_DOWN) {
                    projectile.angle = std::clamp( projectile.angle - 1, 0, 89 );
                }
                // Input: SpaceBar -> Update projectile's shooting power
                else if (playerEvent.context.event.key.key == SDLK_LSHIFT || playerEvent.context.event.key.key == SDLK_RSHIFT) {
                    int nextForce = projectile.currentForce + 50;

                    if (nextForce > projectile.force.max) {
                        nextForce = projectile.force.min;
                    }

                    projectile.currentForce = nextForce;
                }
            }
        }
        else if (playerEvent.context.event.type == SDL_EVENT_KEY_UP) {
            // Reset horizontal movement to 0 when key is released
            if (playerEvent.context.event.key.key == SDLK_A
                 || playerEvent.context.event.key.key == SDLK_LEFT
                 || playerEvent.context.event.key.key == SDLK_D
                 || playerEvent.context.event.key.key == SDLK_RIGHT) {
                velocity.direction.x = 0.0f;
            }

            if (entity->hasComponent<Projectile>()) {
                auto projectile = entity->getComponent<Projectile>();

                // Spawn the projectile
                if (playerEvent.context.event.key.key == SDLK_LSHIFT || playerEvent.context.event.key.key == SDLK_RSHIFT) {
                    ST_Entity& projectileEntity = playerEvent.layer.createEntity();
                    projectileEntity.addComponent<ProjectileTag>();
                    projectileEntity.addComponent<Projectile>( projectile );

                    Transform& projTransform = projectileEntity.addComponent<Transform>();
                    // TODO: Replace hard coded width/height
                    projTransform.position.x = transform.position.x + 32.0f / 2.0f;
                    projTransform.position.y = transform.position.y + 32.0f / 2.0f;
                }
            }
        }

        transform.oldPosition = transform.position;
        transform.position.x += velocity.direction.x * velocity.speed * playerEvent.context.delta;
    }
}