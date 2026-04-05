#pragma once

#include <SDL3/SDL.h>

class ST_RenderHelper
{
public:
    static SDL_FRect getScaledDest( const SDL_FRect& dest, float scale )
    {
        return SDL_FRect{
            dest.x + dest.w * (1 - scale) / 2,
            dest.y + dest.h * (1 - scale) / 2,
            dest.w * scale,
            dest.h * scale
        };
    }

    // This helper center the src frame to dest frame
    static SDL_FRect getCenterDest( const SDL_FRect& src, const SDL_FRect& dest )
    {
        SDL_FRect result;

        result.w = src.w;
        result.h = src.h;

        // get center of dest
        float destCenterX = dest.x + dest.w * 0.5f;
        float destCenterY = dest.y + dest.h * 0.5f;

        // align src's center to dest's center
        result.x = destCenterX - src.w * 0.5f;
        result.y = destCenterY - src.h * 0.5f;

        return result;
    }
};