#pragma once

#include "ST_BaseEvent.h"
#include "ST_Component.h"
#include "ST_Entity.h"


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
    const auto& movementEvent = static_cast<const ST_PlayerActionEvent&>(event);
    ST_Entity* entity = movementEvent.entity;

    if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
        auto& transform = entity->getComponent<Transform>();
        auto& velocity = entity->getComponent<Velocity>();

        // Move the player horizontally when key is pressed
        if (movementEvent.event.type == SDL_EVENT_KEY_DOWN) {
            // If KeyCode is A/LeftArrow, move the entity to the left
            if (movementEvent.event.key.key == SDLK_A || movementEvent.event.key.key == SDLK_LEFT) {
                velocity.direction.x = -1.0f;
            }
            // If KeyCode is D/RightArrow, move the entity to the right
            else if (movementEvent.event.key.key == SDLK_D || movementEvent.event.key.key == SDLK_RIGHT) {
                velocity.direction.x = 1.0f;
            }
        }
        // Reset horizontal movement to 0 when key is released
        else if (movementEvent.event.type == SDL_EVENT_KEY_UP) {
            velocity.direction.x = 0.0f;
        }

        transform.oldPosition = transform.position;
        transform.position.x += velocity.direction.x * velocity.speed * movementEvent.delta;
    }
}

inline void projectileHandler( const ST_BaseEvent& event )
{
    auto& projectileEvent = static_cast<const ST_ProjectileEvent&>(event);

    if (projectileEvent.event.type == SDL_EVENT_KEY_DOWN) {
        // Input: W/UpArrow -> Increase projectile's shooting angle
        if (projectileEvent.event.key.key == SDLK_W || projectileEvent.event.key.key == SDLK_UP) {
            SDL_Log( "increasing shooting angle..." );
        }
        // Input: S/DownArrow -> Decrease projectile's shooting angle
        else if (projectileEvent.event.key.key == SDLK_S || projectileEvent.event.key.key == SDLK_DOWN) {
            SDL_Log( "decreasing shooting angle..." );
        }
        // Input: SpaceBar -> Update projectile's shooting power
        else if (projectileEvent.event.key.key == SDLK_SPACE) {
            SDL_Log( "updating shooting power..." );
        }
    }
}