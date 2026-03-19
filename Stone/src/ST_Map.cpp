#include "ST_Map.h"
#include "ST_TextureManager.h"
#include "ST_Entity.h"

#include <sstream>
#include <tinyxml2.h>

void ST_Map::load( const char* path, SDL_Texture* texture )
{
    m_TileSet = texture;

    tinyxml2::XMLDocument mapDocument;
    mapDocument.LoadFile( path );

    tinyxml2::XMLElement* mapNode = mapDocument.FirstChildElement( "map" );
    m_Width = mapNode->IntAttribute( "width" );
    m_Height = mapNode->IntAttribute( "height" );
    m_TileMap = std::vector<std::vector<int>>( m_Height, std::vector<int>( m_Width, 0 ) ); // Set the dimension of the map

    // Parse map data
    tinyxml2::XMLElement* layerNode = mapNode->FirstChildElement( "layer" );
    tinyxml2::XMLElement* dataNode = layerNode->FirstChildElement( "data" );

    std::string csv = dataNode->GetText();
    std::stringstream ss( csv );

    // Read the CSV data into the tileMap
    std::string tile;
    for (int row = 0; row < m_Height; row++)
    {
        for (int col = 0; col < m_Width; col++)
        {
            if (!std::getline( ss, tile, ',' ))
            {
                // Handle error: not enough tiles in the CSV
                break;
            }
            m_TileMap[row][col] = std::stoi( tile ); // stoi: convert string to int
        }
    }

    tinyxml2::XMLElement* objectGroupNode = mapNode->FirstChildElement( "objectgroup" );

    while (objectGroupNode) {
        const char* layerName = objectGroupNode->Attribute( "name" );

        // parse item spawn points
        if (layerName == std::string( "itemLayer" )) {
            for (tinyxml2::XMLElement* itemNode = objectGroupNode->FirstChildElement( "object" ); itemNode != nullptr; itemNode = itemNode->NextSiblingElement( "object" ))
            {
                float x = itemNode->FloatAttribute( "x" );
                float y = itemNode->FloatAttribute( "y" );
                float w = itemNode->FloatAttribute( "width" );
                float h = itemNode->FloatAttribute( "height" );

                m_ItemSpawnPoints.push_back( SDL_FRect{ x, y, w, h } );
            }
        }

        // parse collision layer
        if (layerName == std::string( "collisionLayer" )) {
            for (tinyxml2::XMLElement* objectNode = objectGroupNode->FirstChildElement( "object" ); objectNode != nullptr; objectNode = objectNode->NextSiblingElement( "object" ))
            {
                Collider collider;
                collider.rect.x = objectNode->FloatAttribute( "x" );
                collider.rect.y = objectNode->FloatAttribute( "y" );
                collider.rect.w = objectNode->FloatAttribute( "width" );
                collider.rect.h = objectNode->FloatAttribute( "height" );
                m_Colliders.push_back( collider );
            }
        }

        objectGroupNode = objectGroupNode->NextSiblingElement( "objectgroup" );
    }
}

//void ST_Map::draw( const Camera& camera )
//{
//    SDL_FRect src{}, dest{};
//
//    dest.w = dest.h = 32;
//
//    int type = 0;
//
//    for (int row = 0; row < m_Height; row++)
//    {
//        for (int col = 0; col < m_Width; col++)
//        {
//            type = m_TileMap[row][col];
//
//            float worldX = static_cast<float>( col ) * dest.w;
//            float worldY = static_cast<float>( row ) * dest.h;
//
//            dest.x = std::round( worldX - camera.view.x );
//            dest.y = std::round( worldY - camera.view.y );
//
//            switch (type)
//            {
//            case 1:
//                // dirt
//                src.x = 0;
//                src.y = 0;
//                src.w = 32;
//                src.h = 32;
//                break;
//            case 2:
//                // grass
//                src.x = 32;
//                src.y = 0;
//                src.w = 32;
//                src.h = 32;
//                break;
//            case 4:
//                src.x = 32;
//                src.y = 32;
//                src.w = 32;
//                src.h = 32;
//                break;
//            default:
//                break;
//            }
//
//            ST_TextureManager::draw( m_TileSet, src, dest );
//        }
//    }
//}