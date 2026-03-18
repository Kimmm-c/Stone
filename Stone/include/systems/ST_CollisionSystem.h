#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"

class ST_CollisionSystem : public ST_ISystem
{
public:
    void update( std::vector<std::unique_ptr<ST_Entity>>& entities, const ST_SystemContext& context ) override
    {
        std::vector<ST_Entity&> collidables = queryCollidables( entities );

        // Update all collider positions
        int len = collidables.size();
        for (auto& entity : collidables) {
            Transform& transform = entity.getComponent<Transform>();
            Collider& collider = entity.getComponent<Collider>();
            collider.rect.x = transform.position.x;
            collider.rect.y = transform.position.y;
        }

        for (int i = 0; i < len; i++) {
            ST_Entity& entityA = collidables[i];
            Collider& colliderA = entityA.getComponent<Collider>();

            for (int j = i + 1; j < len; j++) {
                ST_Entity& entityB = collidables[j];
                Collider& colliderB = entityB.getComponent<Collider>();

                //if (Collision::AABB( colliderA, colliderB ))
                //{
                //    // Handle collision
                //    CollisionEvent collisionEvent{ entityA, entityB, CollisionState::Enter };
                //    world.getEventManager().emit( collisionEvent );
                //}
            }
        }
    }

    std::vector<ST_Entity&> queryCollidables( const std::vector<std::unique_ptr<ST_Entity>>& entities )
    {
        std::vector<ST_Entity&> collidables;

        for (const auto& entity : entities)
        {
            if (entity->hasComponent<Collider>() && entity->hasComponent<Transform>())
            {
                collidables.push_back( *entity );
            }
        }

        return collidables;
    }
};