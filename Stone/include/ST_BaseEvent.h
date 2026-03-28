#pragma once

class ST_Entity;

enum class EventType
{
    Collision,
    PlayerAction,
    ProjectileUpdate
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
    const SDL_Event& event;

    ST_PlayerActionEvent( ST_Entity* entity, const SDL_Event& event )
        : entity( entity )
        , event( event )
    {
        type = EventType::PlayerAction;
    }
};

struct ST_ProjectileEvent : public ST_BaseEvent
{
    ST_Entity* entity = nullptr;
    const SDL_Event& event;

    ST_ProjectileEvent( ST_Entity* entity, const SDL_Event& event )
        : entity( entity )
        , event( event )
    {
        type = EventType::ProjectileUpdate;
    }
};

struct ST_ProjectileAngleEvent : public ST_BaseEvent
{
    ST_Entity* entity = nullptr;
    const SDL_Event& event;

    ST_ProjectileAngleEvent( ST_Entity* entity, const SDL_Event& event )
        : entity( entity )
        , event( event )
    {
        type = EventType::ProjectileUpdate;
    }
};

struct ST_ProjectileChargeEvent : public ST_BaseEvent
{
    ST_Entity* entity = nullptr;
    const SDL_Event& event;

    ST_ProjectileChargeEvent( ST_Entity* entity, const SDL_Event& event )
        : entity( entity )
        , event( event )
    {
        type = EventType::ProjectileUpdate;
    }
};