#include "headers/SATCollider.h"
#include <limits> 

void SATCollider::updatePosition(Vector2d position)
{
    Vector2d delta = position - this->position;
    for (size_t i = 0; i < points.size(); i++)
    {
        points[i] += delta;
    }
    this->position = position;
}

vector<Vector2d> SATCollider::getNormals()
{
    vector<Vector2d> normals;
    for (size_t i = 0; i < points.size(); i++)
    {
        Vector2d p1 = points[i];
        Vector2d p2 = points[(i + 1) % points.size()]; 
        Vector2d edge = p2 - p1;
        normals.push_back(edge.getNormal().getNormalized());
    }
    return normals;
}

Vector2d SATCollider::projectShape(Vector2d axis)
{
    float min = points[0].Dot(axis);
    float max = min;

    for (size_t i = 1; i < points.size(); i++)
    {
        float p = points[i].Dot(axis);
        if (p < min) min = p;
        if (p > max) max = p;
    }
    return Vector2d(min, max);
}

bool SATCollider::checkCollision(SATCollider other)
{
    vector<Vector2d> axes1 = getNormals();
    for (size_t i = 0; i < axes1.size(); i++)
    {
        Vector2d axis = axes1[i];
        Vector2d p1 = projectShape(axis);
        Vector2d p2 = other.projectShape(axis);

        if (p1.y < p2.x || p2.y < p1.x) 
        {
            return false;
        }
    }

    vector<Vector2d> axes2 = other.getNormals();
    for (size_t i = 0; i < axes2.size(); i++)
    {
        Vector2d axis = axes2[i];
        Vector2d p1 = projectShape(axis);
        Vector2d p2 = other.projectShape(axis);

        if (p1.y < p2.x || p2.y < p1.x)
        {
            return false;
        }
    }

    return true;
}