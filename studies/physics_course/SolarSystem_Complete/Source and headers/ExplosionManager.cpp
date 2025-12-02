#include "headers/ExplosionManager.h"
#include <random>

void ExplosionManager::spawnExplosion(sf::Vector2f pos, sf::Color color, int count) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> speedDist(-12.0f, 12.0f);
    std::uniform_real_distribution<float> lifeDist(0.008f, 0.04f);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);

    for (int layer = 0; layer < 3; ++layer) {
        sf::Color layerColor = color;
        int layerCount = count / (layer + 1);

        if (layer == 0) {
            layerColor = sf::Color(255, 255, 200);
        } else if (layer == 1) {
            layerColor.r = std::min(255, (int)layerColor.r + 50);
            layerColor.g = std::min(255, (int)layerColor.g + 50);
            layerColor.b = std::min(255, (int)layerColor.b + 50);
        } else {
            layerColor.r = layerColor.r / 2;
            layerColor.g = layerColor.g / 2;
            layerColor.b = layerColor.b / 2;
        }

        for (int i = 0; i < layerCount; ++i) {
            ExplosionParticle p;
            p.position = pos;

            float angle = angleDist(rng);
            float speed = speedDist(rng) * (0.5f + layer * 0.3f);
            p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

            p.color = layerColor;
            p.life = 1.0f;
            p.decayRate = lifeDist(rng) * (0.8f + layer * 0.4f);

            particles.push_back(p);
        }
    }
}

void ExplosionManager::update() {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->position += it->velocity;

        it->life -= it->decayRate;

        it->velocity *= 0.98f;

        if (it->life <= 0.0f) {
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ExplosionManager::draw(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;

    sf::CircleShape dot;
    for (const auto& p : particles) {
        float size = 3.0f * p.life;
        if (size < 0.0f) size = 0.0f;

        dot.setRadius(size);
        dot.setOrigin(sf::Vector2f(size / 2, size / 2));
        dot.setPosition(p.position);

        sf::Color c = p.color;
        c.a = static_cast<std::uint8_t>(255 * p.life);
        dot.setFillColor(c);

        window.draw(dot, states);
    }
}