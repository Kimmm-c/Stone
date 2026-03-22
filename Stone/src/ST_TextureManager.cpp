#include "ST_TextureManager.h"
#include "ST_Window.h"

std::unordered_map<std::string, SDL_Texture*> ST_TextureManager::m_TextureMap;
std::unique_ptr<ST_Renderer> ST_TextureManager::m_Renderer;

ST_Renderer& ST_TextureManager::createRenderer( const ST_Window& window )
{
    m_Renderer = std::make_unique<ST_Renderer>( window.getNativeWindow() );

    return *m_Renderer.get();
}

SDL_Texture* ST_TextureManager::load( const std::string& path )
{
    return ST_TextureManager::load( path.c_str() );
}

SDL_Texture* ST_TextureManager::load( const char* path )
{
    // Check if texture is already loaded
    auto it = m_TextureMap.find( path );
    if (it != m_TextureMap.end()) {
        return it->second;
    }

    // Surface is an image after being loaded into CPU memory
    // We store it in a surface first, then convert it to a texture to store in GPU memory
    SDL_Surface* tempSurface = IMG_Load( path );

    if (!tempSurface) {
        SDL_Log( "Failed to load image: %s, %s", path, SDL_GetError() );
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface( m_Renderer->getNativeRenderer(), tempSurface );
    SDL_DestroySurface( tempSurface );

    if (!tex) {
        SDL_Log( "Failed to create texture from surface: %s", SDL_GetError() );
        return nullptr;
    }
    else {
        m_TextureMap[path] = tex;
    }

    return tex;
}

// FRect is used for floating point rectangle (x, y, w, h)
void ST_TextureManager::draw( const RenderContext& context )
{
    SDL_RenderTexture( m_Renderer->getNativeRenderer(), context.texture, context.src, context.dest );
}

void ST_TextureManager::clean()
{
    for (auto& pair : m_TextureMap) {
        SDL_DestroyTexture( pair.second );
        pair.second = nullptr;
    }

    m_TextureMap.clear();
}