#pragma once

#include "ST_BaseEvent.h"
#include "ST_Component.h"
#include "ST_Entity.h"
#include "ST_MathHelper.h"

#include <algorithm>

inline void resolveTileCollision( ST_Entity* dynamic, ST_Entity* tile )
{
    if (
        !dynamic->hasComponent<Transform>()
        || !dynamic->hasComponent<Velocity>()
        || !dynamic->hasComponent<Sprite>()
        )
    {
        SDL_Log( "Object missing required component to resolve collision!" );
        return;
    }

    if (
        !tile->hasComponent<Transform>()
        || !tile->hasComponent<Sprite>()
        )
    {
        SDL_Log( "Tile missing required component to resolve collision!" );
        return;
    }

    Transform& objTransform = dynamic->getComponent<Transform>();
    Velocity& objVelocity = dynamic->getComponent<Velocity>();
    Sprite& objSprite = dynamic->getComponent<Sprite>();

    Transform& tileTransform = tile->getComponent<Transform>();
    Sprite& tileSprite = tile->getComponent<Sprite>();

    // TODO: Update this handler to resolve horizontal collision
    objTransform.position = objTransform.oldPosition;

    if (objVelocity.direction.y > 0)
        objVelocity.direction.y = 0;
}

inline void handleDestructiveProjectileTileCollision( ST_Entity* A, ST_Entity* B )
{
    A->addComponent<PendingDestroy>();
    B->addComponent<PendingDestroy>();

    ST_AudioEventQueue::push( { ST_AudioEvent::Type::PlaySfx , "collision" } );
}

inline void handleDestructiveProjectilePlayerCollision( ST_Entity* projectile, ST_Entity* player )
{
    // ignore self-hit
    if (projectile->getComponent<Projectile>().id == player->getComponent<PlayerTag>().id)
        return;

    if (!player->hasComponent<PlayerActionFlags>())
        SDL_Log( "No PlayerActionFlags!" );

    Collider& projCol = projectile->getComponent<Collider>();
    Collider& playerCol = player->getComponent<Collider>();

    float overlap = ST_Collision::overlappingArea( projCol, playerCol );

    if (!projectile->hasComponent<DamageAccumulator>())
        projectile->addComponent<DamageAccumulator>();

    auto& acc = projectile->getComponent<DamageAccumulator>();
    acc.maxOverlap = std::max( acc.maxOverlap, overlap );
    acc.targetPlayer = player;

    player->getComponent<PlayerActionFlags>().isHurt = true;
}

inline void collisionHandler( const ST_BaseEvent& event )
{
    const auto& collision = static_cast<const ST_CollisionEvent&>(event);

    ST_Entity* A = collision.entityA;
    ST_Entity* B = collision.entityB;

    Collider& colliderA = A->getComponent<Collider>();
    Collider& colliderB = B->getComponent<Collider>();

    const std::string& tagA = colliderA.tag;
    const std::string& tagB = colliderB.tag;

    // Collision between a tile with any other object
    if (tagA != "tile" && tagB == "tile") {
        resolveTileCollision( A, B );
    }
    else if (tagA == "tile" && tagB != "tile") {
        resolveTileCollision( B, A );
    }

    // Collision between destructive projectile and tile
    if (tagA == "destructiveProjectile" && tagB == "tile") {
        handleDestructiveProjectileTileCollision( A, B );
    }
    else if (tagA == "tile" && tagB == "destructiveProjectile") {
        handleDestructiveProjectileTileCollision( B, A );
    }

    // Collision between destructive projectile and player
    if (tagA == "destructiveProjectile" && tagB == "player") {
        handleDestructiveProjectilePlayerCollision( A, B );
    }
    else if (tagA == "player" && tagB == "destructiveProjectile") {
        handleDestructiveProjectilePlayerCollision( B, A );
    }
}

inline void handleMovement( ST_Entity* entity, const SDL_Event& event )
{
    if (!entity->hasComponent<Velocity>())
        return;

    auto& velocity = entity->getComponent<Velocity>();
    const auto key = event.key.key;

    const bool isLeft = (key == SDLK_A || key == SDLK_LEFT);
    const bool isRight = (key == SDLK_D || key == SDLK_RIGHT);

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (isLeft) {
            velocity.direction.x = -1.0f;
            velocity.facing = -1.0f;
        }
        else if (isRight) {
            velocity.direction.x = 1.0f;
            velocity.facing = 1.0f;
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        if (isLeft || isRight)
            velocity.direction.x = 0.0f;
    }
}

inline void handleProjectileControl( ST_Entity* entity, const SDL_Event& event )
{
    if (!entity->hasComponent<Projectile>())
        return;

    auto& projectile = entity->getComponent<Projectile>();
    const auto key = event.key.key;

    const bool isUp = (key == SDLK_W || key == SDLK_UP);
    const bool isDown = (key == SDLK_S || key == SDLK_DOWN);
    const bool isShift = (key == SDLK_LSHIFT || key == SDLK_RSHIFT);

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (isUp)
            projectile.angle = std::clamp
            (
                projectile.angle + 1,
                static_cast<int>(projectile.angleRange.x),
                static_cast<int>(projectile.angleRange.y)
            );
        else if (isDown)
            projectile.angle = std::clamp
            (
                projectile.angle - 1,
                static_cast<int>(projectile.angleRange.x),
                static_cast<int>(projectile.angleRange.y)
            );
        else if (isShift)
        {
            int nextForce = projectile.currentForce + projectile.forceIncrement;
            if (nextForce > projectile.forceRange.y)
                nextForce = projectile.forceRange.x;

            projectile.currentForce = nextForce;
        }
    }
}

inline void handleProjectileSpawn( ST_Entity* entity, const ST_PlayerActionEvent& playerEvent )
{
    if (
        !entity->hasComponent<Projectile>()
        || !entity->hasComponent<Transform>()
        )
        return;

    const auto& keyEvent = playerEvent.context.event;
    const auto key = keyEvent.key.key;

    const bool isShift = (key == SDLK_LSHIFT || key == SDLK_RSHIFT);

    if (keyEvent.type == SDL_EVENT_KEY_UP && isShift)
    {
        auto& projectile = entity->getComponent<Projectile>();
        auto& transform = entity->getComponent<Transform>();
        auto& playerVelocity = entity->getComponent<Velocity>();

        ST_Entity& projectileEntity = playerEvent.layer.createEntity();
        projectileEntity.addComponent<PendingProjectileTag>();
        projectileEntity.addComponent<Projectile>( projectile );

        Transform& projTransform = projectileEntity.addComponent<Transform>();

        // TODO: replace hardcoded size
        projTransform.position.x = transform.position.x + 16.0f;
        projTransform.position.y = transform.position.y + 16.0f;

        projectileEntity.addComponent<Velocity>( ST_Vector2D( playerVelocity.facing, 0.0f ) );

        if (!entity->hasComponent<PlayerActionFlags>())
            SDL_Log( "No PlayerActionFlags!" );

        entity->getComponent<PlayerActionFlags>().isThrowing = true;
        ST_AudioEventQueue::push( { ST_AudioEvent::Type::PlaySfx , "whoosh" } );
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
    const auto key = keyEvent.key.key;

    int playerID = entity->getComponent<PlayerTag>().id;

    bool validInput = false;

    if (playerID == 0)
    {
        // Player 1 keys
        validInput = (
            key == SDLK_A || key == SDLK_D ||
            key == SDLK_W || key == SDLK_S ||
            key == SDLK_LSHIFT
            );
    }
    else if (playerID == 1)
    {
        // Player 2 keys
        validInput = (
            key == SDLK_LEFT || key == SDLK_RIGHT ||
            key == SDLK_UP || key == SDLK_DOWN ||
            key == SDLK_RSHIFT
            );
    }

    if (!validInput)
        return;

    handleMovement( entity, keyEvent );
    handleProjectileControl( entity, keyEvent );
    handleProjectileSpawn( entity, playerEvent );
}