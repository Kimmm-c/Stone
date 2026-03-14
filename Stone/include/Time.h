#pragma once

#include <SDL3/SDL.h>

class Time
{
public:
    Time( uint32_t fps ) : m_Fps( fps ) { m_DesiredFrameTime = 1000 / fps; }
    ~Time() = default;

    //float getDelta()
    //{
    //    int currentTicks = SDL_GetTicks();
    //    float delta = (currentTicks - m_Ticks) / 1000.0f; // convert to second
    //    m_Ticks = currentTicks;
    //    return delta;
    //}

    //void synchronizeFrameRate()
    //{
    //    m_ActualFrameTime = SDL_GetTicks() - m_Ticks;
    //    if (m_DesiredFrameTime > m_ActualFrameTime) {
    //        SDL_Delay( m_DesiredFrameTime - m_ActualFrameTime );
    //    }
    //}

private:
    uint32_t m_Fps = 0;
    uint64_t m_Ticks = 0;
    uint64_t m_DesiredFrameTime;
    uint64_t m_ActualFrameTime = 0;

};