#pragma once

#include <SDL3/SDL.h>

class ST_Window;

class ST_Renderer
{
public:
    ST_Renderer( ST_Window* window, const char* name = nullptr );
    ST_Renderer( SDL_Window* window, const char* name = nullptr );
    ~ST_Renderer() = default;

    inline bool exists() const { return m_NativeRenderer != nullptr; }
    inline SDL_Renderer* getNativeRenderer() const { return m_NativeRenderer; }

private:
    SDL_Renderer* m_NativeRenderer = nullptr;
};