#pragma once

#include "ST_Vector2D.h"

class ST_MathHelper
{
public:
    static float map( float val, const ST_Vector2D& originalRange, const ST_Vector2D& targetRange )
    {
        return (val - originalRange.x) / (originalRange.y - originalRange.x) * (targetRange.y - targetRange.x) + targetRange.x;
    }
};