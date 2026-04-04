#pragma once

#include <string>
#include <unordered_map>

#include "ST_Component.h"

class ST_AssetManager
{
public:
    static void loadAnimation( const std::string& clipName, const char* path );
    static void loadAnimation( const std::string& clipName, const std::string& path );
    static const Animation& getAnimation( const std::string& clipName );

private:
    static std::unordered_map<std::string, Animation> m_Animations;
    static Animation loadAnimationFromXML( const char* path );
};