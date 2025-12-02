#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct ExplosionParticle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float life;
    float decayRate;
};

class ExplosionManager {
private:
    std::vector<ExplosionParticle> particles;

public:
    void spawnExplosion(sf::Vector2f pos, sf::Color color, int count);
    void update();
    void draw(sf::RenderWindow& window);
};