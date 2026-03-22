#pragma once

#include <unordered_map>
#include <string>
#include <tinyxml2.h>

#include "ST_Component.h"

class ST_Layer;

struct ST_MapContext
{
    const char* path;
    int tileWidth;
    int tileHeight;

    ST_Layer& parentLayer;
};

class ST_MapManager
{
public:
    ST_MapManager() = default;
    ~ST_MapManager() = default;

    static int loadMap( const ST_MapContext& context, SDL_Texture* texture );
    static void loadColliders( const char* path );

    //void draw( const Camera& camera );

    //inline std::vector<Collider>& getColliders()
    //{
    //    return m_Colliders;
    //}

    //inline std::vector<SDL_FRect>& getItemSpawnPoints()
    //{
    //    return m_ItemSpawnPoints;
    //}

    //inline int getWidth() const
    //{
    //    return m_Width;
    //}

    //inline int getHeight() const
    //{
    //    return m_Height;
    //}


private:
    //SDL_Texture* m_TileSet = nullptr;
    //std::vector<std::vector<int>> m_TileMap;
    //std::vector<Collider> m_Colliders;
    //int m_Width = 0;
    //int m_Height = 0;

    //// item spawn points
    //std::vector<SDL_FRect> m_ItemSpawnPoints;

    static std::unordered_map<std::string, tinyxml2::XMLElement*> m_PathToMap;
};