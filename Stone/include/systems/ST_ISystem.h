#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "ST_Entity.h"

class ST_Layer;
class ST_EventManager;

struct ST_SystemContext
{
    float delta;
    SDL_Event& event;
    ST_EventManager& eventManager;

    Camera& camera;
};

class ST_ISystem
{
public:
    virtual void update( ST_Layer& layer, const ST_SystemContext& context ) = 0;
};