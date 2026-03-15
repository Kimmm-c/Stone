#pragma once

#include <vector>
#include <memory>

#include "ST_ComponentPool.h"
#include "ST_Entity.h"

class ST_Registry
{
public:
    ST_Registry() = default;
    ~ST_Registry() = default;

    ST_Entity& createEntity();
    void destroyEntity();

    template<typename T, typename... Args>
    T& addComponent( const ST_Entity& entity, Args&&... args )
    {
    }


private:
    std::vector<ST_IComponentPool*> m_ComponentPools{};
};