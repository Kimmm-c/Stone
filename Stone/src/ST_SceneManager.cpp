#include "ST_SceneManager.h"
#include "ST_Scene.h"

bool ST_SceneManager::m_ShouldQuit = false;
std::string ST_SceneManager::m_PendingScene;
std::unique_ptr<ST_Scene> ST_SceneManager::m_CurrentScene = nullptr;
std::unordered_map<std::string, ST_GameMetadata> ST_SceneManager::m_SceneConfigs;

void ST_SceneManager::loadScene( const std::string& sceneName, const ST_GameMetadata& meta )
{
    m_SceneConfigs.emplace( sceneName, meta );
}

void ST_SceneManager::render()
{
    if (m_CurrentScene) m_CurrentScene->render();
}

void ST_SceneManager::changeSceneDeferred( const std::string& name )
{
    m_PendingScene = name;
}

void ST_SceneManager::update( const float delta, SDL_Event& e )
{
    if (m_CurrentScene) m_CurrentScene->update( delta, e );

    if (!m_PendingScene.empty()) {
        changeScene( m_PendingScene );
        m_PendingScene.clear();
    }
}

void ST_SceneManager::changeScene( const std::string& name )
{
    auto it = m_SceneConfigs.find( name );
    if (it != m_SceneConfigs.end()) {
        const auto& meta = it->second;
        //create scene obj
        m_CurrentScene = std::make_unique<ST_Scene>( meta );
    }
    else {
        std::cerr << "Scene " << name << " not found" << std::endl;
    }
}