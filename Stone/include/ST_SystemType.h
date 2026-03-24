#pragma once

#include <cstddef>

using SystemTypeID = std::size_t;

// The implementation of this function is moved to a separate cpp file
// so that id can be shared globally even when this header file is included
// in several files.
SystemTypeID getUniqueSystemTypeID();

template <typename T>
SystemTypeID getSystemTypeID()
{
    static SystemTypeID typeID = getUniqueSystemTypeID();
    return typeID;
}
