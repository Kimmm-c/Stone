#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <memory>
#include <string>

class ST_Scene;
class ST_GameMetadata;

class ST_SceneManager
{
public:
    static void loadScene( const std::string& sceneName, const ST_GameMetadata& meta );

    static void render();

    static void changeSceneDeferred( const std::string& name );

    static void update( const float delta, SDL_Event& e );

    inline static void requestQuit() { m_ShouldQuit = true; }

public:
    static bool m_ShouldQuit;

private:
    static void changeScene( const std::string& name );

private:
    static std::string m_PendingScene;
    static std::unique_ptr<ST_Scene> m_CurrentScene;
    static std::unordered_map<std::string, ST_GameMetadata> m_SceneConfigs;
};