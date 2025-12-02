#include "headers/SolarSystem.h"
#include "headers/BackgroundManager.h"
#include "headers/Slider.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1600, 900)), "Solar System Simulator - Mahmoud Mamdouh",
        sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    SolarSystem solarSystem;
    solarSystem.initRealSystem();

    BackgroundManager background;
    background.init();

    sf::RectangleShape uiPanel;
    uiPanel.setFillColor(sf::Color(15, 10, 35, 250));
    uiPanel.setOutlineColor(sf::Color(100, 150, 255, 80));
    uiPanel.setOutlineThickness(-3.0f);

    sf::RectangleShape uiGlow = uiPanel;
    uiGlow.setFillColor(sf::Color(50, 50, 120, 30));
    uiGlow.setOutlineThickness(0);

    Slider speedSlider(0.5f, "SPEED", solarSystem.getFont());
    Slider zoomSlider(0.5f, "ZOOM", solarSystem.getFont());
    Slider sizeSlider(0.5f, "SIZE", solarSystem.getFont());

    sf::View gameView = window.getDefaultView();
    float currentZoom = 0.8f;
    gameView.setSize(sf::Vector2f(window.getSize().x * currentZoom, window.getSize().y * currentZoom));
    gameView.setCenter(sf::Vector2f(0.0f, 0.0f));

    bool isPaused = false;
    bool showInstructions = true;
    sf::Clock dtClock;

    while (window.isOpen()) {
        sf::Vector2u winSize = window.getSize();
        sf::View uiView(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)winSize.x, (float)winSize.y)));

        float panelWidth = 100.0f;
        float uiX = winSize.x - (panelWidth / 2.0f);
        float panelH = (float)winSize.y;

        uiPanel.setSize(sf::Vector2f(panelWidth, panelH));
        uiPanel.setPosition(sf::Vector2f(winSize.x - panelWidth, 0.0f));

        speedSlider.updateLayout(uiX, panelH * 0.2f, 12.0f, 150.0f);
        zoomSlider.updateLayout(uiX, panelH * 0.5f, 12.0f, 150.0f);
        sizeSlider.updateLayout(uiX, panelH * 0.8f, 12.0f, 150.0f);

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                gameView.setSize(sf::Vector2f(
                    static_cast<float>(resized->size.x) * currentZoom,
                    static_cast<float>(resized->size.y) * currentZoom
                ));
            }
            else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::O) solarSystem.toggleOrbits();
                else if (keyPress->code == sf::Keyboard::Key::P) isPaused = !isPaused;
                else if (keyPress->code == sf::Keyboard::Key::R) solarSystem.initRealSystem();
                else if (keyPress->code == sf::Keyboard::Key::H) showInstructions = !showInstructions;
            }
            else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                bool clickedOverUI = mousePress->position.x > (winSize.x - panelWidth);

                if (mousePress->button == sf::Mouse::Button::Left && !clickedOverUI) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePress->position, gameView);
                    solarSystem.spawnRandomPlanet(worldPos);
                }
            }

            speedSlider.handleEvent(*event, window, uiView);
            zoomSlider.handleEvent(*event, window, uiView);
            sizeSlider.handleEvent(*event, window, uiView);
        }

        float dt = dtClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        float targetZoom = 0.5f + (zoomSlider.value * 5.0f);
        currentZoom += (targetZoom - currentZoom) * 0.1f;
        float ratio = (float)winSize.x / winSize.y;
        gameView.setSize(sf::Vector2f(winSize.y * currentZoom * ratio, winSize.y * currentZoom));

        float timeScale = speedSlider.value * 2.5f;
        if (speedSlider.value < 0.05f) timeScale = 0.0f;
        float simDt = isPaused ? 0.0f : (1.0f / 60.0f) * timeScale;

        float visualScale = 0.8f + (sizeSlider.value * 4.0f);

        solarSystem.update(simDt);

        window.clear(sf::Color(10, 8, 20));

        window.setView(gameView);
        background.draw(window, dt);
        solarSystem.draw(window, visualScale);

        window.setView(uiView);

        uiGlow.setSize(uiPanel.getSize());
        uiGlow.setPosition(uiPanel.getPosition());
        sf::RenderStates glowState;
        glowState.blendMode = sf::BlendAdd;
        window.draw(uiGlow, glowState);

        window.draw(uiPanel);

        speedSlider.draw(window);
        zoomSlider.draw(window);
        sizeSlider.draw(window);
        sf::Text info(solarSystem.getFont());
        info.setString("Bodies: " + std::to_string(solarSystem.getBodyCount()));
        info.setCharacterSize(16);
        info.setPosition(sf::Vector2f(20.0f, 20.0f));
        info.setFillColor(sf::Color(200, 220, 255));
        info.setOutlineColor(sf::Color(100, 150, 255, 120));
        info.setOutlineThickness(1.5f);
        info.setStyle(sf::Text::Bold);
        window.draw(info);

        if (showInstructions) {
            sf::RectangleShape instructionPanel;
            instructionPanel.setSize(sf::Vector2f(280.0f, 200.0f));
            instructionPanel.setPosition(sf::Vector2f(20.0f, 60.0f));
            instructionPanel.setFillColor(sf::Color(15, 10, 35, 230));
            instructionPanel.setOutlineColor(sf::Color(100, 150, 255, 150));
            instructionPanel.setOutlineThickness(2.0f);
            window.draw(instructionPanel);

            sf::Text title(solarSystem.getFont());
            title.setString("CONTROLS");
            title.setCharacterSize(18);
            title.setPosition(sf::Vector2f(35.0f, 70.0f));
            title.setFillColor(sf::Color(255, 215, 0));
            title.setStyle(sf::Text::Bold);
            window.draw(title);

            std::vector<std::string> instructions = {
                "LEFT CLICK - Spawn Planet",
                "O - Toggle Orbits",
                "P - Pause/Resume",
                "R - Reset Solar System",
                "H - Hide/Show Help"
            };

            float yPos = 105.0f;
            for (const auto& instruction : instructions) {
                sf::Text line(solarSystem.getFont());
                line.setString(instruction);
                line.setCharacterSize(14);
                line.setPosition(sf::Vector2f(35.0f, yPos));
                line.setFillColor(sf::Color(200, 220, 255));
                window.draw(line);
                yPos += 25.0f;
            }
        }

        window.display();
    }
    return 0;
}