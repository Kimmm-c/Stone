#pragma once

#include "ST_Component.h"

#include <SDL3/SDL.h>

class ST_Collision
{
public:
    static bool AABB( const SDL_FRect& rectA, const SDL_FRect& rectB )
    {
        /*
* 2 bounding boxes are collided when:
* 1. A's right edge is to the right of B's left edge
* 2. A's left edge is to the left of B's right edge
* 3. A's bottom edge is below B's top edge
* 4. A's top edge is above B's bottom edge
*/
        return (rectA.x <= rectB.x + rectB.w &&
                 rectA.x + rectA.w >= rectB.x &&
                 rectA.y <= rectB.y + rectB.h &&
                 rectA.y + rectA.h >= rectB.y);
    }

    static bool AABB( const Collider& colliderA, const Collider& colliderB )
    {
        return AABB( colliderA.rect, colliderB.rect );
    }
};