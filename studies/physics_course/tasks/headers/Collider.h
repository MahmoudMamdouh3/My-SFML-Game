#pragma once
#include "Vector2d.h"

class Collider
{
public:
    float r;
    Vector2d center;

    bool checkCollision(Collider other);
};