#include "headers/CelestialBody.h"
#include <iostream>
#include <algorithm>

CelestialBody::CelestialBody(b2World& world, b2Vec2 pos, float r, float m, sf::Color c,
    std::string n, b2Vec2 vel, bool sun, const sf::Font& font)
    : radius(r), mass(m), color(c), name(n), isSun(sun) {

    b2BodyDef bodyDef;
    bodyDef.type = sun ? b2_staticBody : b2_dynamicBody;
    bodyDef.position = pos;
    bodyDef.linearVelocity = vel;
    bodyDef.linearDamping = 0.0f;
    bodyDef.angularDamping = 0.0f;
    body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = r / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);
    shape.setRadius(r);
    shape.setOrigin(sf::Vector2f(r, r));
    shape.setFillColor(c);

    nameTag = std::make_unique<sf::Text>(font);
    nameTag->setString(name);
    nameTag->setCharacterSize(14);
    nameTag->setFillColor(sf::Color(255, 255, 255, 200));

    sf::FloatRect textRect = nameTag->getLocalBounds();
    nameTag->setOrigin(sf::Vector2f(textRect.size.x / 2.0f, 0.0f));
}

void CelestialBody::update(float dt) {
    if (!body) return;

    b2Vec2 pos = body->GetPosition();
    sf::Vector2f sfPos = toSF(pos);

    shape.setPosition(sfPos);

    if (isSun) {
        pulseTime += dt * 1.5f;
        if (impactFlash > 0.0f) impactFlash -= dt * 2.0f;
        if (impactFlash < 0.0f) impactFlash = 0.0f;
    }
    else {
        if (trail.empty() || length(trail.back() - sfPos) > 10.0f) {
            trail.push_back(sfPos);
            if (trail.size() > MAX_TRAIL_SIZE) trail.erase(trail.begin());
        }
    }
}

void CelestialBody::drawAtmosphere(sf::RenderWindow& window, float visualScale) {
    if (isSun) return;

    sf::Vector2f center = shape.getPosition();
    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;

    float baseRadius = radius * visualScale;

    sf::CircleShape outerGlow(baseRadius * 2.0f);
    outerGlow.setOrigin(sf::Vector2f(outerGlow.getRadius(), outerGlow.getRadius()));
    outerGlow.setPosition(center);
    sf::Color outerColor = color;
    outerColor.a = 30;
    outerGlow.setFillColor(outerColor);
    window.draw(outerGlow, states);

    sf::CircleShape midGlow(baseRadius * 1.6f);
    midGlow.setOrigin(sf::Vector2f(midGlow.getRadius(), midGlow.getRadius()));
    midGlow.setPosition(center);
    sf::Color midColor = color;
    midColor.a = 60;
    midGlow.setFillColor(midColor);
    window.draw(midGlow, states);

    sf::CircleShape innerGlow(baseRadius * 1.3f);
    innerGlow.setOrigin(sf::Vector2f(innerGlow.getRadius(), innerGlow.getRadius()));
    innerGlow.setPosition(center);
    sf::Color innerColor = color;
    innerColor.r = std::min(255, innerColor.r + 50);
    innerColor.g = std::min(255, innerColor.g + 50);
    innerColor.b = std::min(255, innerColor.b + 50);
    innerColor.a = 100;
    innerGlow.setFillColor(innerColor);
    window.draw(innerGlow, states);

    if (name == "Jupiter" || name == "Saturn") {
        for (int i = 0; i < 3; ++i) {
            float bandRadius = baseRadius * (1.1f + i * 0.1f);
            sf::CircleShape band(bandRadius);
            band.setOrigin(sf::Vector2f(bandRadius, bandRadius));
            band.setPosition(center);
            sf::Color bandColor = color;
            bandColor.a = 40 - i * 10;
            band.setFillColor(sf::Color(0, 0, 0, bandColor.a / 4));
            band.setOutlineThickness(1.0f);
            band.setOutlineColor(bandColor);
            window.draw(band, states);
        }
    }

    if (name == "Mars") {
        sf::CircleShape dustGlow(baseRadius * 1.8f);
        dustGlow.setOrigin(sf::Vector2f(dustGlow.getRadius(), dustGlow.getRadius()));
        dustGlow.setPosition(center);
        dustGlow.setFillColor(sf::Color(220, 100, 50, 25));
        window.draw(dustGlow, states);
    }
}

void CelestialBody::drawSunEffects(sf::RenderWindow& window) {
    sf::Vector2f center = shape.getPosition();
    float pulse = 1.0f + std::sin(pulseTime) * 0.08f + impactFlash * 0.3f;
    float fastPulse = 1.0f + std::sin(pulseTime * 2.5f) * 0.04f;
    float slowPulse = 1.0f + std::sin(pulseTime * 0.3f) * 0.15f;

    sf::RenderStates addBlend;
    addBlend.blendMode = sf::BlendAdd;

    sf::CircleShape outerCorona(radius * 8.0f * slowPulse);
    outerCorona.setOrigin(sf::Vector2f(outerCorona.getRadius(), outerCorona.getRadius()));
    outerCorona.setPosition(center);
    outerCorona.setFillColor(sf::Color(255, 80, 0, 25));
    window.draw(outerCorona, addBlend);

    sf::CircleShape midCorona1(radius * 5.5f * pulse);
    midCorona1.setOrigin(sf::Vector2f(midCorona1.getRadius(), midCorona1.getRadius()));
    midCorona1.setPosition(center);
    midCorona1.setFillColor(sf::Color(255, 120, 20, 45));
    window.draw(midCorona1, addBlend);

    sf::CircleShape midCorona2(radius * 4.0f * fastPulse);
    midCorona2.setOrigin(sf::Vector2f(midCorona2.getRadius(), midCorona2.getRadius()));
    midCorona2.setPosition(center);
    midCorona2.setFillColor(sf::Color(255, 150, 30, 35));
    window.draw(midCorona2, addBlend);

    sf::CircleShape innerCorona(radius * 2.8f * pulse);
    innerCorona.setOrigin(sf::Vector2f(innerCorona.getRadius(), innerCorona.getRadius()));
    innerCorona.setPosition(center);
    innerCorona.setFillColor(sf::Color(255, 200, 80, 120));
    window.draw(innerCorona, addBlend);

    sf::CircleShape core(radius * 1.6f);
    core.setOrigin(sf::Vector2f(core.getRadius(), core.getRadius()));
    core.setPosition(center);
    core.setFillColor(sf::Color(255, 255, 220, 255));
    window.draw(core, addBlend);

    float flareIntensity = std::max(0.0f, std::sin(pulseTime * 4.2f) - 0.85f) * 3.0f;
    if (flareIntensity > 0.0f) {
        sf::CircleShape flare(radius * (2.2f + flareIntensity));
        flare.setOrigin(sf::Vector2f(flare.getRadius(), flare.getRadius()));
        flare.setPosition(center);
        flare.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(flareIntensity * 180)));
        window.draw(flare, addBlend);
    }

    float prominencePhase = std::sin(pulseTime * 1.8f);
    if (prominencePhase > 0.7f) {
        sf::CircleShape prominence(radius * 1.8f);
        prominence.setOrigin(sf::Vector2f(prominence.getRadius(), prominence.getRadius()));
        prominence.setPosition(center + sf::Vector2f(std::cos(pulseTime * 3.0f) * radius * 0.5f,
                                                    std::sin(pulseTime * 2.0f) * radius * 0.3f));
        prominence.setFillColor(sf::Color(255, 100, 0, static_cast<std::uint8_t>((prominencePhase - 0.7f) * 300)));
        window.draw(prominence, addBlend);
    }

    if (impactFlash > 0.0f) {
        sf::CircleShape flash(radius * 12.0f * (1.0f + impactFlash * 0.5f));
        flash.setOrigin(sf::Vector2f(flash.getRadius(), flash.getRadius()));
        flash.setPosition(center);
        sf::Color flashColor(255, 255, 255, static_cast<std::uint8_t>(impactFlash * 200));
        flash.setFillColor(flashColor);
        window.draw(flash, addBlend);

        for (int i = 1; i <= 3; ++i) {
            sf::CircleShape ring(radius * (4.0f + i * 2.0f) * impactFlash);
            ring.setOrigin(sf::Vector2f(ring.getRadius(), ring.getRadius()));
            ring.setPosition(center);
            ring.setFillColor(sf::Color(255, 200, 100, static_cast<std::uint8_t>(impactFlash * 150 / i)));
            ring.setOutlineThickness(2.0f);
            ring.setOutlineColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(impactFlash * 100 / i)));
            window.draw(ring, addBlend);
        }
    }
}


void CelestialBody::draw(sf::RenderWindow& window, bool showOrbits, float visualScale) {
    if (isSun) {
        drawSunEffects(window);
        return;
    }

        if (showOrbits && trail.size() > 1) {
            sf::RenderStates trailBlend;
            trailBlend.blendMode = sf::BlendAdd;

            sf::VertexArray lines(sf::PrimitiveType::LineStrip);
            sf::VertexArray glowLines(sf::PrimitiveType::LineStrip);

            for (size_t i = 0; i < trail.size(); ++i) {
                float ratio = (float)i / trail.size();
                float alpha = ratio * 180.0f;
                float glowAlpha = ratio * 60.0f;

                sf::Vertex v;
                v.position = trail[i];
                v.color = color;
                v.color.a = static_cast<std::uint8_t>(alpha);
                lines.append(v);

                sf::Vertex gv;
                gv.position = trail[i];
                gv.color = color;
                gv.color.a = static_cast<std::uint8_t>(glowAlpha);
                glowLines.append(gv);
            }

            sf::RenderStates glowState;
            glowState.blendMode = sf::BlendAdd;
            window.draw(glowLines, glowState);

            window.draw(lines);
        }

    drawAtmosphere(window, visualScale);

    shape.setScale(sf::Vector2f(visualScale, visualScale));
    window.draw(shape);
    shape.setScale(sf::Vector2f(1.0f, 1.0f));

    if (showOrbits) {
        sf::Vector2f pos = shape.getPosition();
        nameTag->setPosition(sf::Vector2f(pos.x, pos.y + (radius * visualScale) + 15.0f));
        window.draw(*nameTag);
    }
}

void CelestialBody::destroyPhysics(b2World& world) {
    if (body) {
        world.DestroyBody(body);
        body = nullptr;
    }
}