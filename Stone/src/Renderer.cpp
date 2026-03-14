#if 0
#include "Renderer.h"
#include "Window.h"

Renderer::Renderer( Window* window, const char* name )
{
    m_NativeRenderer = SDL_CreateRenderer( window->getNativeWindow(), name );
}

#endif