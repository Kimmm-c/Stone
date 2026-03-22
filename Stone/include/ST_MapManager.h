#pragma once

#include <unordered_map>
#include <string>
#include <tinyxml2.h>
#include <memory>

#include "ST_Component.h"

class ST_Layer;

struct ST_MapContext
{
    std::string path;

    int tileWidth;
    int tileHeight;

    bool hasColliders = false;

    ST_Layer& parentLayer;
};

class ST_MapManager
{
public:
    ST_MapManager() = default;
    ~ST_MapManager() = default;

    static int loadMap( const ST_MapContext& context, SDL_Texture* texture );

private:
    static int registerMap( const ST_MapContext& context );
    static int loadTiles( const ST_MapContext& context, SDL_Texture* texture );
    static int loadColliders( const ST_MapContext& context );

private:

    static std::unordered_map<std::string, std::unique_ptr<tinyxml2::XMLDocument>> m_PathToMap;
};