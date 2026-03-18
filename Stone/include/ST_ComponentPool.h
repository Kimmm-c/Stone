#pragma once

#if 0
#include <memory>
#include <vector>
#include <unordered_map>

#include "ST_Entity.h"

class ST_IComponentPool
{
public:
    virtual ~ST_IComponentPool() = default;

    friend class ST_Registry;

private:
    virtual void remove( EntityID entity ) = 0;

};

template<typename T>
class ST_ComponentPool : public ST_IComponentPool
{
public:
    ST_ComponentPool() = default;
    ~ST_ComponentPool() = default;


private:
    template <typename... Args>
    T& add( const ST_Entity& entity, Args&&... args )
    {
        m_Components.emplace_back( T{ std::forward<Args>( args )... } );

        m_EntityToComponentMap.insert( { entity.ID, m_Size } );

        m_Size++;

        return m_Components[m_Size - 1];
    }

    void remove( EntityID entity ) override
    {
        auto it = m_EntityToComponentMap.find( entity );

        if (it == m_EntityToComponentMap.end())
            return;

        int componentIndex = it->second;

        m_Components.erase( m_Components.begin() + componentIndex );

        m_EntityToComponentMap.erase( entity );

        for (auto& [entityId, index] : m_EntityToComponentMap) {
            if (index > componentIndex)
                index--;
        }

        m_Size--;
    }

    inline T& get( uint64_t index ) const { return m_Components[index]; }

private:
    uint64_t m_Size = 0;
    std::vector<T> m_Components{};
    std::unordered_map<EntityID, int> m_EntityToComponentMap{};
};

#endif