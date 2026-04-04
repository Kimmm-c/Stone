#include "ST_AssetManager.h"

#include <iostream>
#include <tinyxml2.h>

std::unordered_map<std::string, Animation> ST_AssetManager::m_Animations;

void ST_AssetManager::loadAnimation( const std::string& clipName, const char* path )
{
    m_Animations[clipName] = loadAnimationFromXML( path );
}

void ST_AssetManager::loadAnimation( const std::string& clipName, const std::string& path )
{
    loadAnimation( clipName, path.c_str() );
}

const Animation& ST_AssetManager::getAnimation( const std::string& clipName )
{
    return m_Animations[clipName];
}

Animation ST_AssetManager::loadAnimationFromXML( const char* path )
{
    Animation animation;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile( path ) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load animation!" << std::endl;
        return animation;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    for (tinyxml2::XMLElement* clip = root->FirstChildElement(); clip != nullptr; clip = clip->NextSiblingElement()) {
        std::string clipName = clip->Name();
        ST_AnimationClip animationClip;

        for (tinyxml2::XMLElement* frame = clip->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement()) {
            SDL_FRect rect;
            rect.x = frame->FloatAttribute( "x" );
            rect.y = frame->FloatAttribute( "y" );
            rect.w = frame->FloatAttribute( "w" );
            rect.h = frame->FloatAttribute( "h" );
            animationClip.frameIndices.push_back( rect );
        }

        animation.clips[clipName] = animationClip;
    }

    if (!animation.clips.empty()) {
        animation.currentClip = animation.clips.begin()->first; // set the first clip as the default
    }

    return animation;
}