#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <memory>
#include <string>

#include "ST_Scene.h"

class ST_SceneManager
{
public:
    ST_SceneManager() = default;
    ~ST_SceneManager() = default;

    ST_Scene& loadScene( const std::string& sceneName, bool isCurrentScene = false )
    {
        auto [it, inserted] = m_Scenes.emplace( sceneName, std::make_unique<ST_Scene>() );

        if (isCurrentScene) m_CurrentScene = it->second.get();

        return *it->second;
    }

    void update( float delta, SDL_Event& event )
    {
        if (m_CurrentScene) m_CurrentScene->update( delta, event );
    }

    void render()
    {
        if (m_CurrentScene) m_CurrentScene->render();
    }

private:
    ST_Scene* m_CurrentScene = nullptr;
    std::unordered_map<std::string, std::unique_ptr<ST_Scene>> m_Scenes;
};