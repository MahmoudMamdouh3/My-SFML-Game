#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Slider {
public:
    sf::RectangleShape track;
    sf::CircleShape knob;
    std::unique_ptr<sf::Text> labelText;
    float value;
    bool isDragging = false;

    Slider(float initialVal, std::string name, const sf::Font& font);

    void updateLayout(float centerX, float topY, float width, float height);

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& uiView);

    void draw(sf::RenderWindow& window);

private:
    void updateValueFromMouseY(float y);
};