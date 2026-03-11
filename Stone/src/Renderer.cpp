#include "Renderer.h"
#include "Window.h"

Renderer::Renderer( Window* window, const char* name = nullptr )
{
    m_NativeRenderer = SDL_CreateRenderer( window->getNativeWindow(), name );
}