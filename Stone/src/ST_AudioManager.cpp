#include "ST_AudioManager.h"

MIX_Mixer* ST_AudioManager::m_Mixer = nullptr;
MIX_Track* ST_AudioManager::m_MusicTrack = nullptr;
MIX_Track* ST_AudioManager::m_SfxTrack;
std::unordered_map<std::string, MIX_Audio*> ST_AudioManager::m_Audio;

void ST_AudioManager::init()
{
    if (MIX_Init() == 0) {
        SDL_Log( "MIX_Init failed" );
        return;
    }

    m_Mixer = MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr );
    if (!m_Mixer) {
        SDL_Log( "MIX_CreateMixerDevice() failed" );
        return;
    }

    m_MusicTrack = MIX_CreateTrack( m_Mixer );
    m_SfxTrack = MIX_CreateTrack( m_Mixer );
    MIX_SetTrackGain( m_MusicTrack, 0.0f );
}

void ST_AudioManager::loadAudio( const std::string& name, const char* path )
{
    if (m_Audio.contains( path )) {
        return;
    }

    auto audioPtr = MIX_LoadAudio( m_Mixer, path, true );
    if (!audioPtr) {
        SDL_Log( "MIX_LoadAudio() failed" );
        return;
    }

    m_Audio.emplace( name, audioPtr );
}

void ST_AudioManager::loadAudio( const std::string& name, const std::string& path )
{
    loadAudio( name, path.c_str() );
}

void ST_AudioManager::playMusic( const std::string& name )
{
    if (MIX_SetTrackAudio( m_MusicTrack, m_Audio[name] ) == 0) {
        SDL_Log( "MIX_SetTrackAudio() failed" );
        return;
    }

    MIX_PlayTrack( m_MusicTrack, -1 ); // -1: loop endlessly
}
void ST_AudioManager::stopMusic()
{
    MIX_StopTrack( m_MusicTrack, 0 );
}

void ST_AudioManager::playSfx( const std::string& name )
{
    if (MIX_SetTrackAudio( m_SfxTrack, m_Audio[name] ) == 0) {
        SDL_Log( "MIX_SetTrackAudio() failed" );
        return;
    }

    MIX_PlayTrack( m_SfxTrack, 0 ); // 0: no loop
}