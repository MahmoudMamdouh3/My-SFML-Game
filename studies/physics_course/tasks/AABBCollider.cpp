#include "headers/AABBCollider.h"

void AABBCollider::updatePosition(Vector2d position)
{
    Vector2d delta = position - this->position;
    ul += delta;
    lr += delta;
    this->position = position;
}

bool AABBCollider::checkCollision(AABBCollider other)
{
    return (ul.x < other.lr.x && lr.x > other.ul.x &&
        ul.y < other.lr.y && lr.y > other.ul.y);
}