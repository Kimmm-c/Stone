#pragma once

#include <cstdint>

static uint64_t id = 0;

using EntityID = uint64_t;

struct ST_Entity
{
    EntityID ID = id++;
};