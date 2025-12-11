#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

class Vector2d
{
public:
    float x;
    float y;

    Vector2d();
    Vector2d(float x, float y);

    Vector2d operator+(Vector2d other) const;
    Vector2d operator-(Vector2d other) const;
    Vector2d operator*(float a) const;
    Vector2d operator/(float a) const;

    Vector2d operator+=(Vector2d other);
    Vector2d operator-=(Vector2d other);
    Vector2d operator*=(float a);
    Vector2d operator=(Vector2d other);

    bool operator==(Vector2d other) const;

    float Dot(Vector2d other) const;
    float getMagnitude() const;
    Vector2d getNormalized() const;
    void normalize();
    Vector2d getNormal() const;

    operator Vector2f();

    void print() const;
};

Vector2d operator*(float a, const Vector2d& vec);