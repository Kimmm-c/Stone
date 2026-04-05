#pragma once

#include <queue>

#include "ST_AudioEvent.h"

class ST_AudioEventQueue
{
public:
    static void push( const ST_AudioEvent& event )
    {
        getQueue().push( event );
    }

    static bool empty()
    {
        return getQueue().empty();
    }

    static ST_AudioEvent pop()
    {
        ST_AudioEvent event = getQueue().front();
        getQueue().pop();
        return event;
    }

private:
    static std::queue<ST_AudioEvent>& getQueue()
    {
        static std::queue<ST_AudioEvent> queue;
        return queue;
    }
};