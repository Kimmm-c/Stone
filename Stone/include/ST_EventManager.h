#pragma once

#include <vector>
#include <functional>

#include "ST_BaseEvent.h"

class ST_EventManager
{
public:
    using Handler = std::function<void( const ST_BaseEvent& )>;

    void emit( const ST_BaseEvent& e ) const
    {
        for (const auto& listener : listeners) {
            listener( e );
        }
    }

    void subscribe( const Handler& callback )
    {
        listeners.emplace_back( callback );
    }

private:
    std::vector<Handler> listeners;
};