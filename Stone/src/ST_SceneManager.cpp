#include "ST_SceneManager.h"

ST_Scene* ST_SceneManager::m_CurrentScene = nullptr;
std::unordered_map<std::string, std::unique_ptr<ST_Scene>> ST_SceneManager::m_Scenes;