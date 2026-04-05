#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Collision.h"
#include "ST_BaseEvent.h"

class ST_CollisionSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();
        std::vector<ST_Entity*> collidables = queryCollidables( entities );

        int len = collidables.size();

        for (int i = 0; i < len; i++) {
            ST_Entity* entityA = collidables[i];

            if (!entityA->isActive())
                continue;

            Collider& colliderA = entityA->getComponent<Collider>();

            for (int j = i + 1; j < len; j++) {
                ST_Entity* entityB = collidables[j];

                if (!entityB->isActive())
                    continue;



                Collider& colliderB = entityB->getComponent<Collider>();

                if (ST_Collision::AABB( colliderA, colliderB ))
                {
                    context.eventManager.emit<ST_CollisionEvent>( ST_CollisionEvent( entityA, entityB, CollisionState::Enter ) );
                }
            }
        }
    }

    std::vector<ST_Entity*> queryCollidables( const std::vector<std::unique_ptr<ST_Entity>>& entities )
    {
        std::vector<ST_Entity*> collidables;

        for (const auto& entity : entities)
        {
            // filter out UI elements
            if (entity->hasComponent<Sprite>()) {
                Sprite& sprite = entity->getComponent<Sprite>();
                if (sprite.renderLayer == RenderLayer::UI)
                    continue;
            }

            if (entity->hasComponent<Collider>()
                 && entity->hasComponent<Transform>()
                 && entity->isActive())
            {
                collidables.push_back( entity.get() );
            }
        }

        return collidables;
    }
};
