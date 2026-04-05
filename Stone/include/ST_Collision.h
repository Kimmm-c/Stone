#pragma once

#include "ST_Component.h"

#include <SDL3/SDL.h>
#include <cmath>

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

    static float overlappingArea( const Collider& colliderA, const Collider& colliderB )
    {
        if (AABB( colliderA.rect, colliderB.rect )) {
            SDL_FRect rectA = colliderA.rect;
            SDL_FRect rectB = colliderB.rect;

            // Compute overlap bounds
            float left = std::max( rectA.x, rectB.x );
            float right = std::min( rectA.x + rectA.w, rectB.x + rectB.w );
            float top = std::max( rectA.y, rectB.y );
            float bottom = std::min( rectA.y + rectA.h, rectB.y + rectB.h );

            float width = right - left;
            float height = bottom - top;

            if (width > 0 && height > 0)
                return width * height;
        }

        return 0.0f;
    }

    static bool pointIsInside( const ST_Vector2D& point, const Collider& collider )
    {
        float leftBound = collider.rect.x;
        float rightBound = collider.rect.x + collider.rect.w;
        float topBound = collider.rect.y;
        float botBound = collider.rect.y + collider.rect.h;

        return (
            point.x >= leftBound
            && point.x <= rightBound
            && point.y >= topBound
            && point.y <= botBound
            );
    }
};