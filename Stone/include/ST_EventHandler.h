#pragma once

#include "ST_BaseEvent.h"
#include "ST_Component.h"
#include "ST_Entity.h"

inline void collisionHandler( const ST_BaseEvent& event )
{
    const auto& collision = static_cast<const ST_CollisionEvent&>(event);

    Collider& colliderA = collision.entityA->getComponent<Collider>();
    Collider& colliderB = collision.entityB->getComponent<Collider>();

    // move the player back to its old position
    if (colliderA.tag == "player" && colliderB.tag == "tile") {
        Transform& playerTransform = collision.entityA->getComponent<Transform>();
        playerTransform.position = playerTransform.oldPosition;
    }
    else if (colliderA.tag == "tile" && colliderB.tag == "player") {
        Transform& playerTransform = collision.entityA->getComponent<Transform>();
        playerTransform.position = playerTransform.oldPosition;
    }


}