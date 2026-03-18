#pragma once

#include <memory>
#include <vector>

#include "ST_Registry.h"
#include "ST_ISystem.h"


class ST_SystemScheduler
{
public:
    ST_SystemScheduler() = default;
    ~ST_SystemScheduler() = default;

    void addSystem( const std::unique_ptr<ST_ISystem>& system )
    {
        m_Systems.push_back( system );
    }

    void update( ST_Registry* registry )
    {
        for (auto& system : m_Systems)
            system->update( registry );
    }

private:
    std::vector<std::unique_ptr<ST_ISystem>> m_Systems{};
};