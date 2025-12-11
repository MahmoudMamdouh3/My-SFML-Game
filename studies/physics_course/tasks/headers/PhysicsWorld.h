#pragma once
#include "Particle.h"
#include <list>
#include <vector>

using namespace std;

class PhysicsWorld
{
private:
    list<Particle*> particles;
public:
    void addParticle(Particle* particle);
    void Update(float deltaTime);

    void checkCollision(); 
    void checktwoCircleCollision();
    void checkAABBCollision();
    void checkSatCollision();

    static const Vector2d gravity;
};