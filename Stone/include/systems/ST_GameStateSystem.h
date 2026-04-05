#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_GameStateSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto* gameState = getGameState( layer );

        if (!gameState || gameState->state == GameState::GameOver)
            return;

        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (
                entity->hasComponent<PlayerTag>()
                && entity->hasComponent<Health>()
                && entity->hasComponent<Transform>()
                )
            {
                auto& health = entity->getComponent<Health>();
                auto& transform = entity->getComponent<Transform>();

                bool deadByHealth = health.current <= 0;
                float maxY = context.camera.view.y + context.camera.view.h;
                bool outOfBounds = (transform.position.y > maxY);

                if (deadByHealth || outOfBounds) {
                    gameState->state = GameState::GameOver;
                    gameState->winner = findOtherPlayer( layer, entity.get() );
                    
                    context.eventManager.emit<ST_GameOverEvent>( ST_GameOverEvent( gameState->winner ) );

                    SDL_Log( "Game has ended!" );
                    return;
                }
            }
        }
    }

private:
    GameStateComponent* getGameState( ST_Layer& layer )
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<GameStateComponent>())
                return &entity->getComponent<GameStateComponent>();
        }

        return nullptr;
    }

    ST_Entity* findOtherPlayer( ST_Layer& layer, ST_Entity* deadPlayer )
    {
        auto& entities = layer.getEntities();

        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerTag>() && entity.get() != deadPlayer)
                return entity.get();
        }

        return nullptr;
    }
};