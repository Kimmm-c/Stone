#pragma once

#include <vector>
#include <functional>

#include "ST_BaseEvent.h"
#include "ST_EventType.h"

constexpr std::size_t MAX_EVENTS = 100;

class ST_EventManager
{
public:
    using Handler = std::function<void( const ST_BaseEvent& )>;

    template<typename T>
    void emit( const ST_BaseEvent& event )
    {
        auto id = getEventTypeID<T>();
        for (auto& handler : m_EventHandlerBuckets[id])
            handler( event );

    }

    template <typename T>
    void subscribe( const Handler& callback )
    {
        m_EventHandlerBuckets[getEventTypeID<T>()].emplace_back( callback );
    }

    void process()
    {
        for (auto& eventHandler : m_EventQueue)
            eventHandler();

        m_EventQueue.clear();
    }

private:
    std::array<std::vector<Handler>, MAX_EVENTS> m_EventHandlerBuckets;
    std::vector<std::function<void()>> m_EventQueue;
};