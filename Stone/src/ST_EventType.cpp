#include "ST_EventType.h"

EventTypeID getUniqueEventTypeID()
{
    static EventTypeID lastID = 0;
    return lastID++;
}