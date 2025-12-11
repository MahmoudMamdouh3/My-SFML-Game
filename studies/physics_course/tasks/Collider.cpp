#include "headers/Collider.h"
#include <iostream>

using namespace std;

bool Collider::checkCollision(Collider other)
{
    return (other.center - center).getMagnitude() < (other.r + r);
}