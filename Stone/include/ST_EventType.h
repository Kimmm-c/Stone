#pragma once

#include <cstddef>

using EventTypeID = std::size_t;

// The implementation of this function is moved to a separate cpp file
// so that id can be shared globally even when this header file is included
// in several files.
EventTypeID getUniqueEventTypeID();

template <typename T>
EventTypeID getEventTypeID()
{
    static EventTypeID typeID = getUniqueEventTypeID();
    return typeID;
}
