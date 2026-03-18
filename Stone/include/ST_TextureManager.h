#pragma once

#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <string>

class ST_TextureManager
{
public:
    static SDL_Texture* load( const char* path );

    static void draw( SDL_Texture* tex, SDL_FRect src, SDL_FRect destS );

    static void clean();

private:
    static std::unordered_map<std::string, SDL_Texture*> textureMap;
};