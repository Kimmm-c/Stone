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