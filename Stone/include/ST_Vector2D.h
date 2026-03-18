#pragma once

class ST_Vector2D
{
public:
    float x;
    float y;

    ST_Vector2D();
    ST_Vector2D( float x, float y );

    // scalar must be on the right side of the multiplication
    ST_Vector2D operator*( float scalar ) const;

    // scalar on the left side
    // using friend to make this function non-member. Otherwise compiler will scream
    friend ST_Vector2D operator*( float scalar, const ST_Vector2D& vec );

    ST_Vector2D& operator+=( const ST_Vector2D& other );
    ST_Vector2D& operator-=( const ST_Vector2D& other );

    ST_Vector2D& operator*=( float scalar );
    ST_Vector2D& operator/=( float scalar );

    ST_Vector2D operator-( const ST_Vector2D& other ) const;
    ST_Vector2D operator+( const ST_Vector2D& other ) const;

    ST_Vector2D operator/( float scalar ) const;
    friend ST_Vector2D operator/( float scalar, const ST_Vector2D& other );

    bool operator==( const ST_Vector2D& other ) const;
    bool operator!=( const ST_Vector2D& other ) const;

    ST_Vector2D operator-() const;

    ST_Vector2D& normalize();
};