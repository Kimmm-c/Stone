#pragma once

#include <SDL3_Mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>

class ST_AudioManager
{
public:
    static void init();
    static void loadAudio( const std::string& name, const char* path );
    static void loadAudio( const std::string& name, const std::string& path );

    static void playMusic( const std::string& name );
    static void stopMusic();

    static void playSfx( const std::string& name );

private:
    static MIX_Mixer* m_Mixer;
    static MIX_Track* m_MusicTrack;

    static MIX_Track* m_SfxTrack;
    static std::unordered_map<std::string, MIX_Audio*> m_Audio;
};