#if 0
#include "ST_Registry.h"

ST_Entity& ST_Registry::createEntity()
{
    ST_Entity entity{};
    return entity;
}

void ST_Registry::destroyEntity( EntityID id )
{
    for (auto& pool : m_ComponentPools)
        pool->remove( id );
}

#endif