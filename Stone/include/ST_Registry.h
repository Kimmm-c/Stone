#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ST_ComponentPool.h"
#include "ST_Entity.h"

class ST_Registry
{
public:
    ST_Registry() = default;
    ~ST_Registry() = default;

    ST_Entity& createEntity();
    void destroyEntity( EntityID id );

    template<typename T, typename... Args>
    T& addComponent( const ST_Entity& entity, Args&&... args )
    {
        auto pool = getPool<T>();

        if (!pool) {
            pool = createPool<T>();
        }

        pool->add( entity, std::forward<Args>( args )... );
    }

private:
    template<typename T>
    ST_ComponentPool<T>* getPool() const
    {
        auto type = std::type_index( typeid(T) );

        auto it = m_PoolMap.find( type );
        if (it == m_PoolMap.end())
            return nullptr;

        return static_cast<ST_ComponentPool<T>*>(it->second);
    }

    template<typename T>
    ST_ComponentPool<T>* createPool()
    {
        std::unique_ptr<ST_ComponentPool<T>> pool = std::make_unique< ST_ComponentPool<T>>();

        m_ComponentPools.push_back( std::move( pool ) );

        ST_ComponentPool<T>* ptr = pool.get();
        m_PoolMap[std::type_index( typeid(T) )] = ptr;

        return ptr;
    }


private:
    std::vector<std::unique_ptr<ST_IComponentPool>> m_ComponentPools{};
    std::unordered_map<std::type_index, ST_IComponentPool*> m_PoolMap{};
};