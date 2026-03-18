#include "ST_Vector2D.h"

#include <cmath>

ST_Vector2D::ST_Vector2D()
    : x( 0 ), y( 0 )
{}

ST_Vector2D::ST_Vector2D( float x, float y )
    : x( x ), y( y )
{}

ST_Vector2D ST_Vector2D::operator*( float scalar ) const
{
    return ST_Vector2D( x * scalar, y * scalar );
}

ST_Vector2D operator*( float scalar, const ST_Vector2D& vec )
{
    return ST_Vector2D( vec.x * scalar, vec.y * scalar );
}

ST_Vector2D& ST_Vector2D::operator+=( const ST_Vector2D& other )
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

ST_Vector2D& ST_Vector2D::normalize()
{
    float length = std::sqrt( x * x + y * y );
    if (length != 0)
    {
        x /= length;
        y /= length;
    }
    return *this;
}

ST_Vector2D& ST_Vector2D::operator-=( const ST_Vector2D& other )
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

ST_Vector2D& ST_Vector2D::operator*=( float scalar )
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

ST_Vector2D& ST_Vector2D::operator/=( float scalar )
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

ST_Vector2D ST_Vector2D::operator-( const ST_Vector2D& other ) const
{
    return ST_Vector2D( x - other.x, y - other.y );
}

ST_Vector2D ST_Vector2D::operator+( const ST_Vector2D& other ) const
{
    return ST_Vector2D( x + other.x, y + other.y );
}

ST_Vector2D ST_Vector2D::operator/( float scalar ) const
{
    return ST_Vector2D( x / scalar, y / scalar );
}

ST_Vector2D operator/( float scalar, const ST_Vector2D& other )
{
    return ST_Vector2D( scalar / other.x, scalar / other.y );
}

bool ST_Vector2D::operator==( const ST_Vector2D& other ) const
{
    return (x == other.x) && (y == other.y);
}

bool ST_Vector2D::operator!=( const ST_Vector2D& other ) const
{
    return !(*this == other);
}

ST_Vector2D ST_Vector2D::operator-() const
{
    return ST_Vector2D( -x, -y );
}