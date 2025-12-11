#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "headers/Renderer.h"
#include "headers/PhysicsWorld.h"
#include "headers/Particle.h"
#include "headers/Vector2d.h"

using namespace sf;
using namespace std;

extern std::string collisionDebugMessage;

void ResetPair(Particle* p1, Particle* p2, float startX1, float startX2, float y, float speed)
{
    if (p1->postion.x < 50 || p1->postion.x > 750 || p2->postion.x < 50 || p2->postion.x > 750)
    {
        if (p1->velocity.x < 0 && p1->postion.x < 100)
        {
            p1->postion = Vector2d(startX1, y);
            p1->velocity = Vector2d(speed, 0);
            p2->postion = Vector2d(startX2, y);
            p2->velocity = Vector2d(-speed, 0);
        }
    }
}

int main()
{
    RenderWindow window(VideoMode({ 800, 600 }), "Physics Engine 2025");
    window.setFramerateLimit(60);

    Renderer renderer;
    Clock clock;

    PhysicsWorld worldCircle;
    PhysicsWorld worldAABB;
    PhysicsWorld worldSAT;

    Font font;
    bool fontLoaded = false;
    if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) fontLoaded = true;

    float y1 = 100;
    Particle* c1 = new Particle(Vector2d(100, y1), 1.0f, 0.0f); c1->velocity = Vector2d(150, 0);
    Particle* c2 = new Particle(Vector2d(700, y1), 1.0f, 0.0f); c2->velocity = Vector2d(-150, 0);
    worldCircle.addParticle(c1); worldCircle.addParticle(c2);

    CircleShape shapeC1(20.f); shapeC1.setOrigin({ 20.f, 20.f }); shapeC1.setFillColor(Color::Green);
    CircleShape shapeC2(20.f); shapeC2.setOrigin({ 20.f, 20.f }); shapeC2.setFillColor(Color::Green);
    renderer.AddDrawable(&shapeC1); renderer.AddDrawable(&shapeC2);

    Text textC(font);
    if (fontLoaded) { textC.setString("Circle Collider"); textC.setCharacterSize(20); textC.setPosition({ 350.f, y1 - 50 }); textC.setFillColor(Color::Green); renderer.AddDrawable(&textC); }

    float y2 = 300;
    Particle* a1 = new Particle(Vector2d(100, y2), 1.0f, 0.0f); a1->velocity = Vector2d(150, 0);
    Particle* a2 = new Particle(Vector2d(700, y2), 1.0f, 0.0f); a2->velocity = Vector2d(-150, 0);
    worldAABB.addParticle(a1); worldAABB.addParticle(a2);

    CircleShape shapeA1(20.f); shapeA1.setOrigin({ 20.f, 20.f }); shapeA1.setFillColor(Color::Yellow);
    CircleShape shapeA2(20.f); shapeA2.setOrigin({ 20.f, 20.f }); shapeA2.setFillColor(Color::Yellow);
    renderer.AddDrawable(&shapeA1); renderer.AddDrawable(&shapeA2);

    Text textA(font);
    if (fontLoaded) { textA.setString("AABB Collider"); textA.setCharacterSize(20); textA.setPosition({ 350.f, y2 - 50 }); textA.setFillColor(Color::Yellow); renderer.AddDrawable(&textA); }

    float y3 = 500;
    Particle* s1 = new Particle(Vector2d(100, y3), 1.0f, 0.0f); s1->velocity = Vector2d(150, 0);
    Particle* s2 = new Particle(Vector2d(700, y3), 1.0f, 0.0f); s2->velocity = Vector2d(-150, 0);
    worldSAT.addParticle(s1); worldSAT.addParticle(s2);

    CircleShape shapeS1(20.f); shapeS1.setOrigin({ 20.f, 20.f }); shapeS1.setFillColor(Color::Cyan);
    CircleShape shapeS2(20.f); shapeS2.setOrigin({ 20.f, 20.f }); shapeS2.setFillColor(Color::Cyan);
    renderer.AddDrawable(&shapeS1); renderer.AddDrawable(&shapeS2);

    Text textS(font);
    if (fontLoaded) { textS.setString("SAT Collider"); textS.setCharacterSize(20); textS.setPosition({ 350.f, y3 - 50 }); textS.setFillColor(Color::Cyan); renderer.AddDrawable(&textS); }

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) window.close();
        }

        Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        worldCircle.Update(dt);
        worldAABB.Update(dt);
        worldSAT.Update(dt);

        worldCircle.checktwoCircleCollision();
        worldAABB.checkAABBCollision();
        worldSAT.checkSatCollision();

        ResetPair(c1, c2, 100, 700, y1, 150);
        ResetPair(a1, a2, 100, 700, y2, 150);
        ResetPair(s1, s2, 100, 700, y3, 150);

        shapeC1.setPosition((Vector2f)c1->postion); shapeC2.setPosition((Vector2f)c2->postion);
        shapeA1.setPosition((Vector2f)a1->postion); shapeA2.setPosition((Vector2f)a2->postion);
        shapeS1.setPosition((Vector2f)s1->postion); shapeS2.setPosition((Vector2f)s2->postion);

        renderer.Render(&window);
    }

    delete c1; delete c2;
    delete a1; delete a2;
    delete s1; delete s2;

    return 0;
}