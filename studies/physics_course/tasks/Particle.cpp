#include "headers/Particle.h"
#include "headers/PhysicsWorld.h" 

Particle::Particle(Vector2d postion, float mass, float drag)
{
    this->mass = mass;
    this->postion = postion;
    this->drag = drag;

    collider.center = postion;
    collider.r = 20;

    collider_aabb.ul = Vector2d(0, 0); 
    collider_aabb.lr = Vector2d(40, 40);
    collider_aabb.updatePosition(postion);

    vector<Vector2d> points;
    points.push_back(Vector2d(0, 0));
    points.push_back(Vector2d(0, 40));
    points.push_back(Vector2d(40, 40));
    points.push_back(Vector2d(40, 0));
    collider_sat.points = points;
    collider_sat.updatePosition(postion);
}

void Particle::Update(float dt)
{
    if (velocity.getMagnitude() > 0)
    {
        Vector2d dragForce = -0.5f * drag * velocity.getMagnitude() * velocity;
        forces += dragForce;
    }

    acceleration = forces * (1.0f / mass);


    velocity += dt * acceleration;
    postion += dt * velocity;

    forces = Vector2d(0, 0);

    collider.center = postion;
    collider_aabb.updatePosition(postion);
    collider_sat.updatePosition(postion);
}

void Particle::addForce(Vector2d force)
{
    forces += force;
}