#pragma once
#include "Vector2d.h"

class AABBCollider
{
public:
    Vector2d ul;
    Vector2d lr;
    Vector2d position;

    void updatePosition(Vector2d position);
    bool checkCollision(AABBCollider other);
};