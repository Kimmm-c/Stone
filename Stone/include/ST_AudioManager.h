#pragma once

#include <SDL3_Mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>

class ST_AudioManager
{
public:
    ST_AudioManager();

    void loadAudio( const std::string& name, const char* path ) const;
    void loadAudio( const std::string& name, const std::string& path ) const;

    void playMusic( const std::string& name ) const;
    void stopMusic() const;

    static void playSfx( const std::string& name );

private:
    MIX_Mixer* m_Mixer = nullptr;
    MIX_Track* m_MusicTrack = nullptr;

    static MIX_Track* m_SfxTrack;
    static std::unordered_map<std::string, MIX_Audio*> m_Audio;
};