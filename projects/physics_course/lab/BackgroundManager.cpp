#include "headers/BackgroundManager.h"
#include <random>
#include <algorithm>
#include <cmath>

void BackgroundManager::init() {
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> posDist(-15000.0f, 15000.0f);
    std::uniform_real_distribution<float> sizeDist(0.5f, 2.5f);
    std::uniform_real_distribution<float> alphaDist(100.0f, 255.0f);

    for (int i = 0; i < 8000; ++i) {
        stars.push_back({
            sf::Vector2f(posDist(rng), posDist(rng)),
            alphaDist(rng),
            sizeDist(rng)
            });
        }

    std::vector<sf::Color> cloudColors = {
        sf::Color(139, 69, 139, 12),
        sf::Color(25, 25, 112, 15),
        sf::Color(139, 0, 69, 10),
        sf::Color(0, 139, 139, 8),
        sf::Color(184, 134, 11, 14),
        sf::Color(75, 0, 130, 9),
        sf::Color(34, 139, 34, 11),
        sf::Color(178, 34, 52, 13),
        sf::Color(72, 61, 139, 10),
        sf::Color(0, 100, 0, 12)
    };
    std::uniform_int_distribution<int> colorIdx(0, (int)cloudColors.size() - 1);
    std::uniform_real_distribution<float> cloudSize(400.0f, 3000.0f);

    for (int i = 0; i < 15; ++i) {
        NebulaCloud c;
        c.pos = sf::Vector2f(posDist(rng), posDist(rng));
        c.radius = cloudSize(rng);
        c.color = cloudColors[colorIdx(rng)];
        clouds.push_back(c);
    }

    std::uniform_real_distribution<float> dustSpeed(-5.0f, 5.0f);
    std::uniform_real_distribution<float> dustSize(0.3f, 1.5f);
    std::uniform_real_distribution<float> dustAlpha(30.0f, 120.0f);

    std::vector<sf::Color> dustColors = {
        sf::Color(200, 200, 255, 80),
        sf::Color(255, 200, 200, 60),
        sf::Color(200, 255, 200, 70),
        sf::Color(255, 255, 150, 50),
        sf::Color(255, 150, 255, 65)
    };

    std::uniform_int_distribution<int> dustColorIdx(0, (int)dustColors.size() - 1);

    for (int i = 0; i < 150; ++i) {
        CosmicDust dust;
        dust.pos = sf::Vector2f(posDist(rng), posDist(rng));
        dust.velocity = sf::Vector2f(dustSpeed(rng) * 0.1f, dustSpeed(rng) * 0.1f);
        dust.size = dustSize(rng);
        dust.alpha = dustAlpha(rng);
        dust.color = dustColors[dustColorIdx(rng)];
        dustParticles.push_back(dust);
    }
}

void BackgroundManager::draw(sf::RenderWindow& window, float dt) {
    timeAccumulator += dt;
    sf::RenderStates states;

    states.blendMode = sf::BlendAdd;
    sf::CircleShape cloudShape;
    for (const auto& c : clouds) {
        cloudShape.setRadius(c.radius);
        cloudShape.setOrigin(sf::Vector2f(c.radius, c.radius));
        cloudShape.setPosition(c.pos);
        cloudShape.setFillColor(c.color);
        window.draw(cloudShape, states);
    }

    states.blendMode = sf::BlendAlpha;
    sf::CircleShape starShape;
    for (size_t i = 0; i < stars.size(); ++i) {
        starShape.setRadius(stars[i].size);
        starShape.setPosition(stars[i].pos);

        float twinkle1 = std::sin(timeAccumulator * 2.0f + i * 0.1f) * 40.0f;
        float twinkle2 = std::sin(timeAccumulator * 0.5f + i * 0.05f) * 20.0f;
        float twinkle3 = std::sin(timeAccumulator * 4.0f + i * 0.2f) * 15.0f;

        float totalTwinkle = twinkle1 + twinkle2 + twinkle3;
        std::uint8_t a = static_cast<std::uint8_t>(std::clamp(stars[i].baseAlpha + totalTwinkle, 30.0f, 255.0f));

        if (i % 50 == 0) {
            starShape.setFillColor(sf::Color(255, 255, 200, a));
        } else if (i % 75 == 0) {
            starShape.setFillColor(sf::Color(200, 220, 255, a));
        } else {
            starShape.setFillColor(sf::Color(255, 255, 255, a));
        }

        window.draw(starShape, states);
    }

    states.blendMode = sf::BlendAlpha;
    sf::CircleShape dustShape;
    for (auto& dust : dustParticles) {
        dustShape.setRadius(dust.size);
        dustShape.setPosition(dust.pos);
        sf::Color dustColor = dust.color;
        dustColor.a = static_cast<std::uint8_t>(dust.alpha + std::sin(timeAccumulator * 0.5f + dust.pos.x * 0.01f) * 20.0f);
        dustShape.setFillColor(dustColor);
        window.draw(dustShape, states);

        dust.pos += dust.velocity * 0.01f;

        if (dust.pos.x < -20000.0f) dust.pos.x = 20000.0f;
        if (dust.pos.x > 20000.0f) dust.pos.x = -20000.0f;
        if (dust.pos.y < -20000.0f) dust.pos.y = 20000.0f;
        if (dust.pos.y > 20000.0f) dust.pos.y = -20000.0f;
    }
}