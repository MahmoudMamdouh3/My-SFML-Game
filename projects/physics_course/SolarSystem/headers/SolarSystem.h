#pragma once
#include "PhysicsUtils.h"
#include "CelestialBody.h"
#include "BackgroundManager.h"
#include "ExplosionManager.h"
#include <memory>
#include <vector>

class SolarSystem {
private:
    std::unique_ptr<b2World> world;
    std::vector<std::unique_ptr<CelestialBody>> bodies;

    ExplosionManager explosions;

    sf::Font font;

    bool showOrbits = true;
    int spawnedCount = 1;

    b2Vec2 calculateOrbitVelocity(b2Vec2 centerPos, float centerMass, b2Vec2 orbiterPos);

public:
    SolarSystem();
    const sf::Font& getFont() const { return font; }

    void initRealSystem();

    void spawnRandomPlanet(sf::Vector2f worldPos);

    void applyGravity();
    void checkCollisions();
    void update(float dt);

    void draw(sf::RenderWindow& window, float visualScale);

    void toggleOrbits() { showOrbits = !showOrbits; }
    size_t getBodyCount() const { return bodies.size(); }
};