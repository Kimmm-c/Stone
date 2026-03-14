#pragma once

#include <memory>

class Window;
class Renderer;
class SceneManager;
class Time;

class Game
{
public:
    Game() = default;
    ~Game() = default;

    void init( const char* title, int windowWidth, int windowHeight, bool fullScreen, uint32_t frameRate );
    void run();

private:
    void processInput();
    void update( float delta );
    void render();

    inline bool isRunning() const { return m_IsRunning; }

private:
    bool m_IsRunning = false;

    std::unique_ptr<SceneManager> m_SceneManager{};

    std::unique_ptr<Window> m_Window = nullptr;
    std::unique_ptr<Renderer> m_Renderer = nullptr;
    std::unique_ptr<Time> m_Time = nullptr;
};