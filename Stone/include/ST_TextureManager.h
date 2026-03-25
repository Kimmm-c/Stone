#pragma once

#include <SDL3_image/SDL_image.h>
#include <memory>
#include <unordered_map>
#include <string>

#include "ST_Renderer.h"

struct RenderContext
{
    // TODO (Optimization): Implement engine's custom Texture class
    SDL_Texture* texture;
    SDL_FRect* src;
    SDL_FRect* dest;

    int alphaValue = -1;
};

class ST_TextureManager
{
public:
    static ST_Renderer& createRenderer( const ST_Window& window );

    static SDL_Texture* load( const char* path );
    static SDL_Texture* load( const std::string& path );

    static void draw( const RenderContext& context );

    static void clean();

    static inline ST_Renderer* getRenderer() { return m_Renderer.get(); }

private:
    static std::unordered_map<std::string, SDL_Texture*> m_TextureMap;
    static std::unique_ptr<ST_Renderer> m_Renderer;
};