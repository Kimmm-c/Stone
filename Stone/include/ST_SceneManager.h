#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <memory>
#include <string>

#include "ST_Scene.h"

class SceneManager
{
public:
    void loadScene( const char* sceneName, bool isCurrentScene = false )
    {
        m_Scenes[sceneName] = std::make_unique<Scene>();

        if (isCurrentScene) m_CurrentScene = m_Scenes[sceneName].get();
    }

    void update( float delta, const SDL_Event& event )
    {
        if (m_CurrentScene) m_CurrentScene->update( delta, event );
    }

    void render( const std::unique_ptr<Renderer>& renderer )
    {
        if (m_CurrentScene) m_CurrentScene->render( renderer );
    }

private:
    Scene* m_CurrentScene = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_Scenes{};
};