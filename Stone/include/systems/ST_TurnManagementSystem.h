#pragma once

#include "ST_ISystem.h"
#include "ST_Component.h"
#include "ST_Layer.h"

class ST_TurnManagementSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        auto& entities = layer.getEntities();
        // Init turn at the beginning of the game
        if (m_Players.empty()) {
            for (auto& entity : entities) {
                if (entity->hasComponent<PlayerTag>())
                    m_Players.push_back( entity.get() );
            }

            if (!m_Players.empty()) {
                m_ActivePlayerIndex = 0;
            }
        }

        if (m_Players.empty())
            return;

        bool projectileActive = false;

        for (auto& entity : entities) {
            if (entity->hasComponent<ProjectileTag>()) {
                projectileActive = true;
                break;
            }
        }

        switch (m_State) {
        case TurnState::Aiming:
        {
            // Set active player to disable the other player's input
            setActivePlayer( layer );

            if (projectileActive)
                m_State = TurnState::Resolving;

            break;
        }
        case TurnState::Resolving:
        {   // While projectile is in the air, disable both users' input
            clearActivePlayer();

            // Assign next player's turn and switch state to aiming again
            if (!projectileActive) {
                // Add delay so camera doesn't instantly snap back to next player
                if (!m_WaitingForDelay) {
                    m_WaitingForDelay = true;
                    m_Timer = 0.0f;
                }

                m_Timer += context.delta;

                if (m_Timer >= m_ResolveDelay) {
                    nextPlayer();
                    m_State = TurnState::Aiming;
                    m_WaitingForDelay = false;
                }
            }

            break;
        }
        }
    }

private:
    void nextPlayer()
    {
        m_ActivePlayerIndex = (m_ActivePlayerIndex + 1) % m_Players.size();
    }

    void setActivePlayer( ST_Layer& layer )
    {
        for (size_t i = 0; i < m_Players.size(); i++) {
            ST_Entity* player = m_Players[i];

            if (i == m_ActivePlayerIndex) {
                player->addComponent<ActivePlayerTag>();
            }
            else {
                if (player->hasComponent<ActivePlayerTag>())
                    player->deactivate<ActivePlayerTag>();
            }
        }
    }

    void clearActivePlayer()
    {
        for (auto* player : m_Players)
        {
            if (player->hasComponent<ActivePlayerTag>())
                player->deactivate<ActivePlayerTag>();
        }
    }

private:
    enum class TurnState
    {
        Aiming,
        Resolving
    };

    TurnState m_State = TurnState::Aiming;

    std::vector<ST_Entity*> m_Players;
    size_t m_ActivePlayerIndex = 0;
    float m_ResolveDelay = 0.5f;
    float m_Timer = 0.0f;
    bool m_WaitingForDelay = false;
};