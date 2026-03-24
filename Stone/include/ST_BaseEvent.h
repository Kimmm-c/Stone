#pragma once

class ST_Entity;

enum class EventType
{
    Collision,
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