#pragma once

#include <SDL3/SDL.h>
#include <vector>

struct ST_AnimationClip
{
    std::vector<SDL_FRect> frameIndices;
};