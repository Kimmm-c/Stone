#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "ST_Entity.h"

struct ST_SystemContext
{
    float delta;
    SDL_Event& event;
    
    Camera& camera;
};

class ST_ISystem
{
public:
    virtual void update( std::vector<std::unique_ptr<ST_Entity>>& entities, const ST_SystemContext& context ) = 0;
};