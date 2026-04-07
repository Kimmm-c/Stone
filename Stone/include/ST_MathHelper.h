#pragma once

#include "ST_Vector2D.h"

#include <random>

class ST_MathHelper
{
public:
    static float map( float val, const ST_Vector2D& originalRange, const ST_Vector2D& targetRange )
    {
        return (val - originalRange.x) / (originalRange.y - originalRange.x) * (targetRange.y - targetRange.x) + targetRange.x;
    }

    static int getIntInRange( int min, int max )
    {
        std::uniform_int_distribution<> dist( min, max );
        return dist( m_Generator );
    }

private:
    static std::random_device m_RandomDevice;
    static std::mt19937 m_Generator;
};