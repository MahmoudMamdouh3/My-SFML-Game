#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Star {
    sf::Vector2f pos;
    float baseAlpha;
    float size;
};

struct NebulaCloud {
    sf::Vector2f pos;
    float radius;
    sf::Color color;
};

struct CosmicDust {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    float size;
    float alpha;
    sf::Color color;
};

class BackgroundManager {
private:
    std::vector<Star> stars;
    std::vector<NebulaCloud> clouds;
    std::vector<CosmicDust> dustParticles;
    float timeAccumulator = 0.0f;

public:
    void init();
    void draw(sf::RenderWindow& window, float dt);
};