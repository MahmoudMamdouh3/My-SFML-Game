#include "headers/Vector2d.h"

Vector2d::Vector2d()
{
    this->x = 0;
    this->y = 0;
}

Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2d Vector2d::operator+(Vector2d other) const
{
    return Vector2d(x + other.x, y + other.y);
}

Vector2d Vector2d::operator-(Vector2d other) const
{
    return Vector2d(x - other.x, y - other.y);
}

Vector2d Vector2d::operator*(float a) const
{
    return Vector2d(x * a, y * a);
}

Vector2d Vector2d::operator/(float a) const
{
    if (a == 0) return Vector2d(0, 0);
    return Vector2d(x / a, y / a);
}

Vector2d Vector2d::operator+=(Vector2d other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2d Vector2d::operator-=(Vector2d other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2d Vector2d::operator*=(float a)
{
    x *= a;
    y *= a;
    return *this;
}

Vector2d Vector2d::operator=(Vector2d other)
{
    x = other.x;
    y = other.y;
    return *this;
}

bool Vector2d::operator==(Vector2d other) const
{
    return x == other.x && y == other.y;
}

float Vector2d::Dot(Vector2d other) const
{
    return (x * other.x) + (y * other.y);
}

float Vector2d::getMagnitude() const
{
    return sqrt(x * x + y * y);
}

Vector2d Vector2d::getNormalized() const
{
    float mag = getMagnitude();
    if (mag == 0) return Vector2d(0, 0);
    return Vector2d(x / mag, y / mag);
}

void Vector2d::normalize()
{
    float mag = getMagnitude();
    if (mag > 0)
    {
        x /= mag;
        y /= mag;
    }
}

Vector2d Vector2d::getNormal() const
{
    return Vector2d(-y, x);
}

Vector2d::operator Vector2f()
{
    return Vector2f(x, y);
}

void Vector2d::print() const
{
    cout << "X: " << x << " Y: " << y << endl;
}

Vector2d operator*(float a, const Vector2d& vec)
{
    return Vector2d(vec.x * a, vec.y * a);
}