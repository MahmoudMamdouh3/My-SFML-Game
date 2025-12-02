#pragma once
#include "PhysicsUtils.h"
#include <string>
#include <memory>
#include <vector>

class CelestialBody {
public:
    b2Body* body = nullptr;
    float radius = 0.f;
    float mass = 0.f;
    sf::Color color;
    std::string name;

    std::vector<sf::Vector2f> trail;

    sf::CircleShape shape;
    std::unique_ptr<sf::Text> nameTag;

    bool isSun = false;
    bool markedForDeath = false;
    float pulseTime = 0.0f;
    float impactFlash = 0.0f;

    CelestialBody(b2World& world, b2Vec2 pos, float r, float m, sf::Color c,
        std::string n, b2Vec2 vel, bool sun, const sf::Font& font);

    void update(float dt);
    void draw(sf::RenderWindow& window, bool showOrbits, float visualScale);
    void destroyPhysics(b2World& world);

private:
    void drawAtmosphere(sf::RenderWindow& window, float visualScale);
    void drawSunEffects(sf::RenderWindow& window);
};