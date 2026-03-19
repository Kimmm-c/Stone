#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include "ST_Component.h"

class ST_Map
{
public:
    ST_Map() = default;
    ~ST_Map() = default;

    void load( const char* path, SDL_Texture* texture );
    //void draw( const Camera& camera );

    inline std::vector<Collider>& getColliders()
    {
        return m_Colliders;
    }

    inline std::vector<SDL_FRect>& getItemSpawnPoints()
    {
        return m_ItemSpawnPoints;
    }

    inline int getWidth() const
    {
        return m_Width;
    }

    inline int getHeight() const
    {
        return m_Height;
    }


private:
    SDL_Texture* m_TileSet = nullptr;
    std::vector<std::vector<int>> m_TileMap;
    std::vector<Collider> m_Colliders;
    int m_Width = 0;
    int m_Height = 0;

    // item spawn points
    std::vector<SDL_FRect> m_ItemSpawnPoints;
};