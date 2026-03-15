#pragma once

#include <memory>

#include "ST_Window.h"
#include "ST_Renderer.h"
#include "ST_Time.h"

class SceneManager;

class ST_Game
{
public:
    ST_Game( const char* title, int windowWidth, int windowHeight, bool fullScreen, uint32_t frameRate );
    ~ST_Game() = default;

    void init();
    void run();

private:
    void processInput();
    void update( float delta );
    void render();

    inline bool isRunning() const { return m_IsRunning; }

private:
    bool m_IsRunning = false;

    std::unique_ptr<SceneManager> m_SceneManager{};

    std::unique_ptr<ST_Window> m_Window = nullptr;
    std::unique_ptr<ST_Renderer> m_Renderer = nullptr;
    std::unique_ptr<ST_Time> m_Time = nullptr;
};