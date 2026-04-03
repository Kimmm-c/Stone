#include "ST_Window.h"

ST_Window::ST_Window( const char* title, int width, int height, bool fullscreen )
    : m_Width( width )
    , m_Height( height )
{
    Uint32 flags = 0;

    if (fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_InitSubSystem( SDL_INIT_VIDEO ) == 1)
    {
        m_NativeWindow = SDL_CreateWindow(
            title,
            width, height,
            flags
        );
    }
}