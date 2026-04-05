#pragma once

class ST_Entity;

enum class EventType
{
    Collision,
    PlayerAction,
    ProjectileUpdate,
    GameOver,
    MouseInteraction
};

struct ST_BaseEvent
{
    EventType type{};
    virtual ~ST_BaseEvent() = default; // In C++, always make the destructor virtual from a class you are deriving from
};

enum class CollisionState
{
    Enter,
    Stay,
    Exit
};

struct ST_CollisionEvent : public ST_BaseEvent
{
    ST_Entity* entityA = nullptr;
    ST_Entity* entityB = nullptr;

    CollisionState state{};
    ST_CollisionEvent( ST_Entity* entityA, ST_Entity* entityB, CollisionState state )
        : entityA( entityA )
        , entityB( entityB )
        , state( state )
    {
        type = EventType::Collision;
    }
};

struct ST_PlayerActionEvent : public ST_BaseEvent
{
    ST_Entity* entity = nullptr;
    ST_Layer& layer;
    const ST_SystemContext& context;

    ST_PlayerActionEvent( ST_Entity* entity, ST_Layer& layer, const ST_SystemContext& context )
        : entity( entity )
        , layer( layer )
        , context( context )
    {
        type = EventType::PlayerAction;
    }
};

struct ST_GameOverEvent : public ST_BaseEvent
{
    ST_Entity* winner;

    ST_GameOverEvent( ST_Entity* winnerEntity )
        : winner( winnerEntity )
    {
        type = EventType::GameOver;
    }
};

enum class MouseInteractionState
{
    Pressed,
    Released,
    Cancel
};

struct ST_MouseInteractionEvent : public ST_BaseEvent
{
    ST_Entity* entity = nullptr;
    MouseInteractionState state{};

    ST_MouseInteractionEvent( ST_Entity* entity, MouseInteractionState state )
        : entity( entity )
        , state( state )
    {
        type = EventType::MouseInteraction;
    }
};