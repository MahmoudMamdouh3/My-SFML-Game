#pragma once
#include "Vector2d.h"
#include <vector>

using namespace std;

class SATCollider
{
    Vector2d position;
public:
    vector<Vector2d> points;
    void updatePosition(Vector2d position);

    bool checkCollision(SATCollider other);
    vector<Vector2d> getNormals();
    Vector2d projectShape(Vector2d axis);
};