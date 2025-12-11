#include "headers/PhysicsWorld.h"
#include <iostream>
#include <string>

std::string collisionDebugMessage = "";

const Vector2d PhysicsWorld::gravity = Vector2d(0, 9.8f);

void PhysicsWorld::addParticle(Particle* particle)
{
    particles.push_back(particle);
}

void PhysicsWorld::Update(float deltaTime)
{
    collisionDebugMessage = "";

    list<Particle*>::iterator it;
    for (it = particles.begin(); it != particles.end(); it++)
    {
        (*it)->Update(deltaTime);
    }
}

void ResolveCollision(Particle* p1, Particle* p2)
{
    Vector2d x1 = p1->postion;
    Vector2d x2 = p2->postion;
    Vector2d v1 = p1->velocity;
    Vector2d v2 = p2->velocity;
    float m1 = p1->mass;
    float m2 = p2->mass;

    Vector2d distanceVec = x1 - x2;
    float distance = distanceVec.getMagnitude();
    float totalRadius = p1->collider.r + p2->collider.r;

    if (distance == 0) return;

    float overlap = totalRadius - distance;
    Vector2d normal = distanceVec.getNormalized();

    Vector2d correction = normal * (overlap / 2.0f);
    p1->postion += correction;
    p2->postion -= correction;

    p1->collider.center = p1->postion;
    p2->collider.center = p2->postion;
    p1->collider_aabb.updatePosition(p1->postion);
    p1->collider_sat.updatePosition(p1->postion);
    p2->collider_aabb.updatePosition(p2->postion);
    p2->collider_sat.updatePosition(p2->postion);

    float denominator = distance * distance;
    float dotProduct = (v1 - v2).Dot(x1 - x2);

    float commonScalar = dotProduct / denominator;

    float m1_factor = (2 * m2) / (m1 + m2);
    Vector2d v1_new = v1 - (distanceVec * (m1_factor * commonScalar));

    float dotProduct2 = (v2 - v1).Dot(x2 - x1);
    float m2_factor = (2 * m1) / (m1 + m2);
    Vector2d v2_new = v2 - ((x2 - x1) * (m2_factor * (dotProduct2 / denominator)));

    p1->velocity = v1_new;
    p2->velocity = v2_new;

    collisionDebugMessage = "Bounce!";
}

void PhysicsWorld::checktwoCircleCollision()
{
    list<Particle*>::iterator it1;
    for (it1 = particles.begin(); it1 != particles.end(); it1++)
    {
        list<Particle*>::iterator it2 = it1;
        it2++;

        for (; it2 != particles.end(); it2++)
        {
            Particle* p1 = *it1;
            Particle* p2 = *it2;

            if (p1->collider.checkCollision(p2->collider))
            {
                ResolveCollision(p1, p2);
            }
        }
    }
}

void PhysicsWorld::checkAABBCollision()
{
    list<Particle*>::iterator it1;
    for (it1 = particles.begin(); it1 != particles.end(); it1++)
    {
        list<Particle*>::iterator it2 = it1;
        it2++;

        for (; it2 != particles.end(); it2++)
        {
            Particle* p1 = *it1;
            Particle* p2 = *it2;

            if (p1->collider_aabb.checkCollision(p2->collider_aabb))
            {
                ResolveCollision(p1, p2);
            }
        }
    }
}

void PhysicsWorld::checkSatCollision()
{
    list<Particle*>::iterator it1;
    for (it1 = particles.begin(); it1 != particles.end(); it1++)
    {
        list<Particle*>::iterator it2 = it1;
        it2++;

        for (; it2 != particles.end(); it2++)
        {
            Particle* p1 = *it1;
            Particle* p2 = *it2;

            if (p1->collider_sat.checkCollision(p2->collider_sat))
            {
                ResolveCollision(p1, p2);
            }
        }
    }
}

void PhysicsWorld::checkCollision()
{
    checktwoCircleCollision();
}