#pragma once

#include "ST_ISystem.h"
#include "ST_Layer.h"
#include "ST_AudioEventQueue.h"
#include "ST_AudioManager.h"

class ST_AudioSystem : public ST_ISystem
{
public:
    void update( ST_Layer& layer, const ST_SystemContext& context ) override
    {
        while (!ST_AudioEventQueue::empty())
        {
            ST_AudioEvent event = ST_AudioEventQueue::pop();

            switch (event.type)
            {
            case ST_AudioEvent::Type::PlaySfx:
                ST_AudioManager::playSfx( event.name );
                break;

            case ST_AudioEvent::Type::PlayMusic:
                ST_AudioManager::playMusic( event.name );
                break;

            case ST_AudioEvent::Type::StopMusic:
                ST_AudioManager::stopMusic();
                break;
            }
        }
    }
};