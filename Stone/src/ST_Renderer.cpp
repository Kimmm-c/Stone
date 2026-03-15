#include "ST_Renderer.h"
#include "ST_Window.h"

ST_Renderer::ST_Renderer( ST_Window* window, const char* name )
{
    m_NativeRenderer = SDL_CreateRenderer( window->getNativeWindow(), name );
}