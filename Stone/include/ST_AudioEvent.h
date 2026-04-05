#pragma once

#include <string>

struct ST_AudioEvent
{
    enum class Type
    {
        PlaySfx,
        PlayMusic,
        StopMusic
    };

    Type type;
    std::string name;
};