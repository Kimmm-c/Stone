#pragma once

#include <SDL3/SDL.h>

class Window;

class Renderer
{
public:
    Renderer( Window* window, const char* name = nullptr );
    ~Renderer() = default;

    inline bool exists() const { return m_NativeRenderer != nullptr; }

private:
    SDL_Renderer* m_NativeRenderer = nullptr;
};