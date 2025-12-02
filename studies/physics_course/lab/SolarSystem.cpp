#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26495) 
#endif

#include "headers/SolarSystem.h"
#include "headers/BackgroundManager.h"
#include "headers/ExplosionManager.h"
#include "headers/Slider.h"
#include "headers/PhysicsUtils.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <string>






SolarSystem::SolarSystem() {
    world = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            (void)font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        }
    }
}

b2Vec2 SolarSystem::calculateOrbitVelocity(b2Vec2 centerPos, float centerMass, b2Vec2 orbiterPos) {
    b2Vec2 diff = orbiterPos - centerPos;
    float r = diff.Length();
    if (r < 0.1f) return b2Vec2(0, 0);

    float speed = std::sqrt(G_CONST * centerMass / r);
    b2Vec2 tangent(-diff.y, diff.x);
    tangent.Normalize();
    return b2Vec2(tangent.x * speed, tangent.y * speed);
}

void SolarSystem::initRealSystem() {
    bodies.clear();
    spawnedCount = 1;

    b2Vec2 sunPos(0.0f, 0.0f);
    bodies.push_back(std::make_unique<CelestialBody>(
        *world, sunPos, 60.0f, 50000.0f, sf::Color(255, 200, 50), "Sun", b2Vec2(0, 0), true, font
    ));

    CelestialBody* sun = bodies[0].get();

    struct PlanetData {
        float dist, radius, mass;
        sf::Color color;
        std::string name;
    };

    std::vector<PlanetData> planets = {
        {55.0f, 8.0f, 0.3f, sf::Color(160, 160, 160), "Mercury"},
        {70.0f, 14.0f, 1.0f, sf::Color(255, 198, 73), "Venus"},
        {90.0f, 15.0f, 1.0f, sf::Color(100, 149, 237), "Earth"},
        {115.0f, 12.0f, 0.7f, sf::Color(205, 92, 92), "Mars"},
        {150.0f, 35.0f, 5.0f, sf::Color(210, 180, 140), "Jupiter"},
        {190.0f, 30.0f, 4.0f, sf::Color(238, 232, 170), "Saturn"}
    };

    for (const auto& p : planets) {
        b2Vec2 planetPos(p.dist, 0.0f);
        b2Vec2 vel = calculateOrbitVelocity(sunPos, sun->mass, planetPos);

        bodies.push_back(std::make_unique<CelestialBody>(
            *world, planetPos, p.radius, p.mass, p.color, p.name, vel, false, font
        ));
    }
}


void SolarSystem::spawnRandomPlanet(sf::Vector2f worldPos) {
    if (bodies.empty()) return;

    CelestialBody* sun = bodies[0].get();
    b2Vec2 sunPos = sun->body->GetPosition();
    b2Vec2 spawnPos = toB2(worldPos);

    b2Vec2 diff = spawnPos - sunPos;
    float distance = diff.Length();
    if (distance < (sun->radius / SCALE + 1.0f)) {
        std::cout << "Too close to Sun to spawn!" << std::endl;
        return;
    }

    b2Vec2 vel = calculateOrbitVelocity(sunPos, sun->mass, spawnPos);

    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> sizeDist(8.0f, 20.0f);
    std::uniform_real_distribution<float> massDist(0.3f, 2.0f);
    std::uniform_int_distribution<int> colorIdxDist(0, (int)PLANET_PALETTE.size() - 1);

    float r = sizeDist(rng);
    float m = massDist(rng);
    sf::Color c = PLANET_PALETTE[colorIdxDist(rng)];
    std::string name = "Planet-" + std::to_string(spawnedCount++);

    bodies.push_back(std::make_unique<CelestialBody>(
        *world, spawnPos, r, m, c, name, vel, false, font
    ));
}

void SolarSystem::applyGravity() {
    if (bodies.empty()) return;

    CelestialBody* sun = bodies[0].get();
    b2Vec2 sunPos = sun->body->GetPosition();

    for (size_t i = 1; i < bodies.size(); ++i) {
        CelestialBody* planet = bodies[i].get();
        if (!planet->body) continue;

        b2Vec2 planetPos = planet->body->GetPosition();
        b2Vec2 diff = sunPos - planetPos;
        float distSq = diff.LengthSquared();

        if (distSq < MIN_DISTANCE) distSq = MIN_DISTANCE;

        float dist = std::sqrt(distSq);

        float forceMag = G_CONST * sun->mass * planet->mass / distSq;

        b2Vec2 forceDir = diff;
        forceDir.Normalize();
        b2Vec2 force = b2Vec2(forceMag * forceDir.x, forceMag * forceDir.y);

        planet->body->ApplyForceToCenter(force, true);
    }
}

void SolarSystem::checkCollisions() {
    if (bodies.empty()) return;
    CelestialBody* sun = bodies[0].get();
    sf::Vector2f sunPos = toSF(sun->body->GetPosition());

    for (size_t i = 1; i < bodies.size(); ++i) {
        CelestialBody* planet = bodies[i].get();
        sf::Vector2f planetPos = toSF(planet->body->GetPosition());

        float dist = length(planetPos - sunPos);

        if (dist < (sun->radius + planet->radius * 0.3f)) {
            explosions.spawnExplosion(planetPos, planet->color, 250);
            sun->impactFlash = 1.0f;
            planet->markedForDeath = true;
        }
    }
}

void SolarSystem::update(float dt) {
    applyGravity();
    const int32 velocityIterations = 8;
    const int32 positionIterations = 3;
    world->Step(dt, velocityIterations, positionIterations);

    for (auto it = bodies.begin(); it != bodies.end(); ) {
        if ((*it)->markedForDeath) {
            (*it)->destroyPhysics(*world);
            it = bodies.erase(it);
        }
        else {
            ++it;
        }
    }

    checkCollisions();
    explosions.update();

    for (auto& body : bodies) {
        body->update(dt);
    }
}

void SolarSystem::draw(sf::RenderWindow& window, float visualScale) {
    for (auto& body : bodies) {
        body->draw(window, showOrbits, visualScale);
    }
    explosions.draw(window);
}