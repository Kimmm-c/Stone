#pragma once

#include <cstddef>

// size_t is an unsigned long long (very large)
using ComponentTypeID = std::size_t;

// Why using inline for a free function:
// Avoid Multiple Definitions: When a function is defined in a header file,
// including that header in multiple translation units can lead to multiple definitions of the same function.
// Marking the function as inline allows the compiler to treat it as a single definition across all translation
// units, preventing linker errors.

// Why using static for lastID:
// Persistent State: The static keyword gives the variable lastID internal linkage and static storage duration.
inline ComponentTypeID getComponentTypeID()
{
    static ComponentTypeID lastID = 0;
    return lastID++;
}

// Template is inline by default
// getComponentTypeID<T> will return a unique ID for each type T
// Example: getComponentTypeID<Position>() will always return 1
template <typename T>
ComponentTypeID getComponentTypeID()
{
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}