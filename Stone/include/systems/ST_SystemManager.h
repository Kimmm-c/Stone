#pragma once

#include <memory>
#include <array>
#include <unordered_set>
#include <iostream>

#include "ST_SystemType.h"
#include "ST_Layer.h"

constexpr std::size_t MAX_SYSTEMS = 100;

class ST_SystemManager
{
public:
    ST_SystemManager() = default;
    ~ST_SystemManager() = default;

    friend class ST_Scene;

private:
    template<typename T>
    T& addSystem()
    {
        auto id = getSystemTypeID<T>();
        auto system = std::make_unique<T>();

        m_Systems[id] = std::move( system );

        return *static_cast<T*>(m_Systems[id].get());
    }

    template<typename... Systems>
    void registerLayer( ST_Layer& layer )
    {
        (m_LayerBuckets[getSystemTypeID<Systems>()].insert( &layer ), ...);
    }

    void update( const ST_SystemContext& context )
    {
        for (int systemIndex = 0; systemIndex < MAX_SYSTEMS; systemIndex++) {
            if (!m_Systems[systemIndex])
                continue;

            ST_ISystem& system = *m_Systems[systemIndex];

            auto& layerBucket = m_LayerBuckets[systemIndex];
            for (auto* layer : layerBucket) {
                system.update( *layer, context );
            }
        }
    }

private:
    std::array<std::unique_ptr<ST_ISystem>, MAX_SYSTEMS> m_Systems;
    std::array<std::unordered_set<ST_Layer*>, MAX_SYSTEMS> m_LayerBuckets;
};