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
    void emit( const ST_BaseEvent& event ) const
    {
        auto id = getEventTypeID<T>();

        auto& handlerBucket = m_EventHandlerBuckets[id];

        for (auto& handler : handlerBucket)
            handler( event );
    }

    template <typename T>
    void subscribe( const Handler& callback )
    {
        m_EventHandlerBuckets[getEventTypeID<T>()].emplace_back( callback );
    }

private:
    std::array<std::vector<Handler>, MAX_EVENTS> m_EventHandlerBuckets;
};