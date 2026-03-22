#include "ST_MapManager.h"
#include "ST_Layer.h"
#include "ST_Component.h"
#include "ST_Vector2D.h"

#include <sstream>

std::unordered_map<std::string, std::unique_ptr<tinyxml2::XMLDocument>> ST_MapManager::m_PathToMap;


int ST_MapManager::loadMap( const ST_MapContext& mapContext, const ST_SpriteSheetContext& spriteSheetContext )
{
    // Return if map has been loaded already
    if (m_PathToMap.find( mapContext.path ) != m_PathToMap.end())
        return 1;

    // Load and register map
    if (ST_MapManager::registerMap( mapContext ) < 0) {
        SDL_Log( "Failed to load XLM document" );
        return -1;
    }

    if (ST_MapManager::loadTiles( mapContext, spriteSheetContext ) < 0) {
        SDL_Log( "Failed to load map tiles!" );
        return -1;
    }

    if (mapContext.hasColliders) {
        if (ST_MapManager::loadColliders( mapContext ) < 0) {
            SDL_Log( "Failed to load collider!" );
            return -1;
        }
    }

    return 1;
}

int ST_MapManager::loadColliders( const ST_MapContext& context )
{
    auto it = m_PathToMap.find( context.path );

    if (it == m_PathToMap.end())
        return -1;

    tinyxml2::XMLElement* mapNode = it->second->FirstChildElement( "map" );
    tinyxml2::XMLElement* objectGroupNode = mapNode->FirstChildElement( "objectgroup" );

    while (objectGroupNode) {
        const char* layerName = objectGroupNode->Attribute( "name" );

        //// parse item spawn points
        //if (layerName == std::string( "itemLayer" )) {
        //    for (tinyxml2::XMLElement* itemNode = objectGroupNode->FirstChildElement( "object" ); itemNode != nullptr; itemNode = itemNode->NextSiblingElement( "object" ))
        //    {
        //        float x = itemNode->FloatAttribute( "x" );
        //        float y = itemNode->FloatAttribute( "y" );
        //        float w = itemNode->FloatAttribute( "width" );
        //        float h = itemNode->FloatAttribute( "height" );

        //        m_ItemSpawnPoints.push_back( SDL_FRect{ x, y, w, h } );
        //    }
        //}

        // parse collision layer
        if (layerName == std::string( "collisionLayer" )) {
            for (tinyxml2::XMLElement* objectNode = objectGroupNode->FirstChildElement( "object" ); objectNode != nullptr; objectNode = objectNode->NextSiblingElement( "object" ))
            {
                Collider collider;
                collider.rect.x = objectNode->FloatAttribute( "x" );
                collider.rect.y = objectNode->FloatAttribute( "y" );
                collider.rect.w = objectNode->FloatAttribute( "width" );
                collider.rect.h = objectNode->FloatAttribute( "height" );

                ST_Entity& mapCollider = context.parentLayer.createEntity();

                Transform transform = Transform( ST_Vector2D( collider.rect.x, collider.rect.y ), ST_Vector2D( 0.0f, 0.0f ), 0.0f, 1.0f );
                mapCollider.addComponent<Transform>( transform );
                Collider& mapColliderComp = mapCollider.addComponent<Collider>( "wall" );
                mapColliderComp.rect = collider.rect;

                // visualize the collider
                SDL_Texture* colliderTexture = ST_TextureManager::load( std::string( ASSET_PATH ) + "spritesheet.png" );
                SDL_FRect colliderSrc{ 0, 32, 32, 32 };
                SDL_FRect collerDest{ collider.rect.x, collider.rect.y, collider.rect.w, collider.rect.h };
                mapCollider.addComponent<Sprite>( colliderTexture, colliderSrc, collerDest );
            }
        }

        objectGroupNode = objectGroupNode->NextSiblingElement( "objectgroup" );
    }
}

int ST_MapManager::registerMap( const ST_MapContext& context )
{
    std::unique_ptr<tinyxml2::XMLDocument> mapDocument = std::make_unique<tinyxml2::XMLDocument>();
    tinyxml2::XMLError isLoaded = mapDocument->LoadFile( context.path.c_str() );

    if (isLoaded != tinyxml2::XML_SUCCESS) {
        SDL_Log( mapDocument->ErrorStr() );
        return -1;
    }

    m_PathToMap[context.path] = std::move( mapDocument );

    return 1;
}

int ST_MapManager::loadTiles( const ST_MapContext& mapContext, const ST_SpriteSheetContext& spriteSheetContext )
{
    // Create entities with visual representation
    auto it = m_PathToMap.find( mapContext.path );
    tinyxml2::XMLElement* mapNode = it->second->FirstChildElement( "map" );

    int mapWidth = mapNode->IntAttribute( "width" );
    int mapHeight = mapNode->IntAttribute( "height" );

    tinyxml2::XMLElement* layerNode = mapNode->FirstChildElement( "layer" );
    tinyxml2::XMLElement* dataNode = layerNode->FirstChildElement( "data" );

    std::string csv = dataNode->GetText();
    std::stringstream ss( csv );

    // Read the CSV data into the tileMap
    std::string tile;
    for (int row = 0; row < mapHeight; row++)
    {
        for (int col = 0; col < mapWidth; col++)
        {
            if (!std::getline( ss, tile, ',' ))
            {
                // Handle error: not enough tiles in the CSV
                break;
            }


            int textureIndex = std::stoi( tile ) - 1;

            // Texture index smaller than zero means the there's no tile in this cell
            if (textureIndex >= 0) {
                // Create entity
                ST_Entity& tileEntity = mapContext.parentLayer.createEntity();

                // Set up Sprite component for each tile
                Sprite sprite;
                sprite.texture = spriteSheetContext.texture;
                sprite.src.w = sprite.dest.w = mapContext.tileWidth;
                sprite.src.h = sprite.dest.h = mapContext.tileHeight;

                int colIndex = textureIndex % spriteSheetContext.sheetWidth;
                int rowIndex = textureIndex / spriteSheetContext.sheetWidth;

                sprite.src.x = colIndex * mapContext.tileWidth;
                sprite.src.y = rowIndex * mapContext.tileHeight;

                tileEntity.addComponent<Sprite>( sprite );
                tileEntity.addComponent<MapTile>( row, col );
            }
        }
    }

    return 1;
}