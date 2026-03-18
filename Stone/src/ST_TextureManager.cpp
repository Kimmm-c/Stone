#include "ST_TextureManager.h"
#include "ST_Game.h"

// Use extern to access the global game object. This avoid circular dependency between Game and TextureManager.
extern ST_Game* game;

std::unordered_map<std::string, SDL_Texture*> ST_TextureManager::textureMap;

SDL_Texture* ST_TextureManager::load( const char* path )
{
    // Check if texture is already loaded
    auto it = textureMap.find( path );
    if (it != textureMap.end()) {
        return it->second;
    }

    // Surface is an image after being loaded into CPU memory
    // We store it in a surface first, then convert it to a texture to store in GPU memory
    SDL_Surface* tempSurface = IMG_Load( path );

    if (!tempSurface) {
        SDL_Log( "Failed to load image: %s, %s", path, SDL_GetError() );
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface( game->getRenderer()->getNativeRenderer(), tempSurface );
    SDL_DestroySurface( tempSurface );

    if (!tex) {
        SDL_Log( "Failed to create texture from surface: %s", SDL_GetError() );
        return nullptr;
    }
    else {
        textureMap[path] = tex;
    }

    return tex;
}

// FRect is used for floating point rectangle (x, y, w, h)
void ST_TextureManager::draw( SDL_Texture* tex, SDL_FRect src, SDL_FRect destS )
{
    SDL_RenderTexture( game->getRenderer()->getNativeRenderer(), tex, &src, &destS );
}

void ST_TextureManager::clean()
{
    for (auto& pair : textureMap) {
        SDL_DestroyTexture( pair.second );
        pair.second = nullptr;
    }

    textureMap.clear();
}