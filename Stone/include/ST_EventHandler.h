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

        transform.position.y = transform.oldPosition.y;

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

inline void handleMovement( ST_Entity* entity, const SDL_Event& event )
{
    if (!entity->hasComponent<Velocity>()) return;

    auto& velocity = entity->getComponent<Velocity>();
    const auto key = event.key.key;

    const bool isLeft = (key == SDLK_A || key == SDLK_LEFT);
    const bool isRight = (key == SDLK_D || key == SDLK_RIGHT);

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (isLeft)       velocity.direction.x = -1.0f;
        else if (isRight) velocity.direction.x = 1.0f;
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        if (isLeft || isRight)
            velocity.direction.x = 0.0f;
    }
}

inline void handleProjectileControl( ST_Entity* entity, const SDL_Event& event )
{
    if (!entity->hasComponent<Projectile>()) return;

    auto& projectile = entity->getComponent<Projectile>();
    const auto key = event.key.key;

    const bool isUp = (key == SDLK_W || key == SDLK_UP);
    const bool isDown = (key == SDLK_S || key == SDLK_DOWN);
    const bool isShift = (key == SDLK_LSHIFT || key == SDLK_RSHIFT);

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (isUp)
            projectile.angle = std::clamp( projectile.angle + 1, 0, 89 );
        else if (isDown)
            projectile.angle = std::clamp( projectile.angle - 1, 0, 89 );
        else if (isShift)
        {
            int nextForce = projectile.currentForce + 50;
            if (nextForce > projectile.force.max)
                nextForce = projectile.force.min;

            projectile.currentForce = nextForce;
        }
    }
}

inline void handleProjectileSpawn( ST_Entity* entity, const ST_PlayerActionEvent& playerEvent )
{
    if (!entity->hasComponent<Projectile>() || !entity->hasComponent<Transform>())
        return;

    const auto& keyEvent = playerEvent.context.event;
    const auto key = keyEvent.key.key;

    const bool isShift = (key == SDLK_LSHIFT || key == SDLK_RSHIFT);

    if (keyEvent.type == SDL_EVENT_KEY_UP && isShift)
    {
        auto& projectile = entity->getComponent<Projectile>();
        auto& transform = entity->getComponent<Transform>();

        ST_Entity& projectileEntity = playerEvent.layer.createEntity();
        projectileEntity.addComponent<PendingProjectileTag>();
        projectileEntity.addComponent<Projectile>( projectile );

        Transform& projTransform = projectileEntity.addComponent<Transform>();

        // TODO: replace hardcoded size
        projTransform.position.x = transform.position.x + 16.0f;
        projTransform.position.y = transform.position.y + 16.0f;
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

    const auto& keyEvent = playerEvent.context.event;

    handleMovement( entity, keyEvent );
    handleProjectileControl( entity, keyEvent );
    handleProjectileSpawn( entity, playerEvent );
}