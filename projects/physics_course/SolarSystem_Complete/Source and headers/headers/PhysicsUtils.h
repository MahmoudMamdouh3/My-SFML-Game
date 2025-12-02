#pragma once
#include <SFML/Graphics.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26495)
#endif

#include <box2d/box2d.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <vector>
#include <cmath>

constexpr float SCALE = 30.0f;
constexpr float G_CONST = 2.0f;
constexpr float REAL_G = 6.67430e-11f;
constexpr size_t MAX_TRAIL_SIZE = 2000;
constexpr float MIN_DISTANCE = 0.1f;

constexpr float AU_TO_METERS = 1.496e11f;
constexpr float SOLAR_MASS = 1.989e30f;
constexpr float EARTH_MASS = 5.972e24f;
constexpr float JUPITER_MASS = 1.898e27f;

inline sf::Vector2f toSF(const b2Vec2& v) {
    return sf::Vector2f(v.x * SCALE, v.y * SCALE);
}

inline b2Vec2 toB2(const sf::Vector2f& v) {
    return b2Vec2(v.x / SCALE, v.y / SCALE);
}

inline float length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

const std::vector<sf::Color> PLANET_PALETTE = {
    sf::Color(64, 224, 208),
    sf::Color(255, 105, 180),
    sf::Color(138, 43, 226),
    sf::Color(255, 140, 0),
    sf::Color(50, 205, 50),
    sf::Color(30, 144, 255),
    sf::Color(255, 215, 0),
    sf::Color(220, 20, 60)
};