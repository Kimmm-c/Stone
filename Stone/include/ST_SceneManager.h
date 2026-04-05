#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <memory>
#include <string>

#include "ST_Scene.h"

class ST_SceneManager
{
public:
    static ST_Scene& loadScene( const std::string& sceneName, bool isCurrentScene = false )
    {
        auto [it, inserted] = m_Scenes.emplace( sceneName, std::make_unique<ST_Scene>() );

        if (isCurrentScene) m_CurrentScene = it->second.get();

        return *it->second;
    }

    static void update( float delta, SDL_Event& event )
    {
        if (m_CurrentScene) m_CurrentScene->update( delta, event );
    }

    static void render()
    {
        if (m_CurrentScene) m_CurrentScene->render();
    }

    static void changeScene( const std::string& sceneName )
    {
        auto it = m_Scenes.find( sceneName );

        if (it != m_Scenes.end()) {
            m_CurrentScene = it->second.get();
        }
        else {
            SDL_Log( "Can't find scene!" );
        }
    }

    static ST_Scene& getCurrentScene()
    {
        return *m_CurrentScene;
    }

private:
    static ST_Scene* m_CurrentScene;
    static std::unordered_map<std::string, std::unique_ptr<ST_Scene>> m_Scenes;
};