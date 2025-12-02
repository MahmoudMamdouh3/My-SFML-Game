#include "headers/Slider.h"
#include <algorithm>

Slider::Slider(float initialVal, std::string name, const sf::Font& font)
    : value(initialVal) {

    track.setFillColor(sf::Color(40, 40, 60, 200));
    track.setOutlineColor(sf::Color(100, 150, 255, 150));
    track.setOutlineThickness(2.0f);

    knob.setFillColor(sf::Color(200, 220, 255));
    knob.setOutlineColor(sf::Color(255, 255, 255, 200));
    knob.setOutlineThickness(3.0f);

    labelText = std::make_unique<sf::Text>(font);
    labelText->setString(name);
    labelText->setCharacterSize(14);
    labelText->setFillColor(sf::Color(180, 220, 255));
    labelText->setOutlineColor(sf::Color(100, 150, 255, 100));
    labelText->setOutlineThickness(1.0f);
    labelText->setStyle(sf::Text::Style::Bold);

    sf::FloatRect bounds = labelText->getLocalBounds();
    labelText->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y));
}

void Slider::updateLayout(float centerX, float topY, float width, float height) {
    track.setSize(sf::Vector2f(width, height));
    track.setOrigin(sf::Vector2f(width / 2.0f, 0.0f));
    track.setPosition(sf::Vector2f(centerX, topY));

    float knobRadius = width * 1.5f;
    knob.setRadius(knobRadius);
    knob.setOrigin(sf::Vector2f(knobRadius, knobRadius));

    float yPos = topY + height * (1.0f - value);
    knob.setPosition(sf::Vector2f(centerX, yPos));

    labelText->setPosition(sf::Vector2f(centerX, topY - 20.0f));
}

void Slider::updateValueFromMouseY(float y) {
    float top = track.getPosition().y;
    float height = track.getSize().y;

    float raw = 1.0f - ((y - top) / height);

    value = std::clamp(raw, 0.0f, 1.0f);
}

bool Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& uiView) {
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos = window.mapPixelToCoords(mousePress->position, uiView);

            sf::FloatRect bounds = track.getGlobalBounds();
            bounds.size.x += 60.0f;
            bounds.position.x -= 30.0f;
            bounds.size.y += 20.0f;
            bounds.position.y -= 10.0f;

            if (bounds.contains(mPos)) {
                isDragging = true;
                updateValueFromMouseY(mPos.y);
                return true;
            }
        }
    }
    else if (const auto* mouseRel = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRel->button == sf::Mouse::Button::Left) {
            isDragging = false;
        }
    }
    else if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (isDragging) {
            sf::Vector2f mPos = window.mapPixelToCoords(mouseMove->position, uiView);
            updateValueFromMouseY(mPos.y);
            return true;
        }
    }
    return false;
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(knob);
    window.draw(*labelText);
}