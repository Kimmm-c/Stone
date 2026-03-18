#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "ST_Entity.h"

class ST_ISystem
{
public:
    virtual void update( std::vector<std::unique_ptr<ST_Entity>>& entities, float delta, SDL_Event& event ) = 0;
};