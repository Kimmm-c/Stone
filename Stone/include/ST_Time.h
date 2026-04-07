#pragma once

#include <SDL3/SDL.h>

class ST_Time
{
public:
    ST_Time( Uint32 fps ) : m_Fps( fps )
    {
        m_DesiredFrameTime = 1000 / fps;
        m_Ticks = SDL_GetTicks();
    }

    ~ST_Time() = default;

    float getDelta()
    {
        Uint64 currentTicks = SDL_GetTicks();
        float delta = (currentTicks - m_Ticks) / 1000.0f; // convert to second
        delta = std::min( delta, 0.033f ); // cap at 30 FPS
        m_Ticks = currentTicks;
        return delta;
    }

    void synchronizeFrameRate()
    {
        m_ActualFrameTime = SDL_GetTicks() - m_Ticks;
        if (m_DesiredFrameTime > m_ActualFrameTime) {
            SDL_Delay( m_DesiredFrameTime - m_ActualFrameTime );
        }
    }

private:
    Uint32 m_Fps = 0;
    Uint64 m_Ticks;
    int m_DesiredFrameTime;
    Uint64 m_ActualFrameTime = 0;

};