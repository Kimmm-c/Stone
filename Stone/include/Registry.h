#pragma once

#include <vector>
#include <memory>

#include "ComponentPool.h"
#include "Component.h"

class Registry
{
public:
    Registry() = default;
    ~Registry() = default;

    Entity& createEntity();
    void destroyEntity();

    template<typename T, typename... Args>
    T& addComponent( const Entity& entity, Args&&... args )
    {

    }

    template<typename T>
    T& getComponent( const Entity& entity )
    {

    }

    template<typename T>
    bool hasComponent( const Entity& entity )
    {

    }

private:
    std::vector<std::unique_ptr<IComponentPool>> m_ComponentPools{};
};