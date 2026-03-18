#pragma once

#include <bitset>
#include <cstddef>
#include <array>

#include "ST_ComponentType.h"

// constexpr creates the constant at compile time
// benefits: performance (no runtime overhead) and safety (immutable)
// difference between constexpr and #define: constexpr respects scope and type
constexpr std::size_t MAX_COMPONENTS = 32;


// bitset[0] means the entity has component type 0
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

// Each element of the array is a pointer to a component
using ComponentArray = std::array<void*, MAX_COMPONENTS>;


class ST_Entity
{
public:
    inline bool isActive() const { return active; }
    inline void destroy() { active = false; }

    // entity.hasComponent<Position>() -> true/false
    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... Args>
    T& addComponent( Args&&... args )
    {
        T* component( new T{ std::forward<Args>( args )... } );

        componentBitSet[getComponentTypeID<T>()] = true;
        componentArray[getComponentTypeID<T>()] = component;

        return *component;
    }

    template<typename T>
    T& getComponent()
    {
        auto component = componentArray[getComponentTypeID<T>()];

        return *static_cast<T*>(component);
    }

    template<typename T>
    void deactivate()
    {
        componentBitSet[getComponentTypeID<T>()] = false;
    }

private:
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};
};