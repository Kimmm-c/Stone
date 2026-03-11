#pragma once

#include <SDL3/SDL.h>

class Window
{
public:
    Window( const char* title, int width, int height, bool fullscreen );
    ~Window() = default;

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    inline const char* getWindowTitle() const { return SDL_GetWindowTitle( m_NativeWindow ); }
    inline SDL_Window* getNativeWindow() const { return m_NativeWindow; }
    inline bool exists() const { return m_NativeWindow != nullptr; }

private:
    int m_Width{};
    int m_Height{};

    SDL_Window* m_NativeWindow = nullptr;
};