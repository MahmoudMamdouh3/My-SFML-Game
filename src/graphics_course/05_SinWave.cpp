#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// --- Vertex Shader (Calculates the Wave) ---
const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    
    uniform float u_Time;

    void main() {
        // Wave Parameters
        float amplitude = 0.5;  // Height of the wave
        float frequency = 10.0; // How many waves fit on screen
        float speed = 2.0;      // Animation speed

        // Calculate Y based on X and Time
        // This 'displaces' the flat line into a wave
        float y = sin(aPos.x * frequency + u_Time * speed) * amplitude;

        gl_Position = vec4(aPos.x, y, aPos.z, 1.0);
    }
)";

// --- Fragment Shader (Color) ---
const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue Color
    }
)";

void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "\n" << infoLog << "\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM::" << type << "\n" << infoLog << "\n";
        }
    }
}

int main() {
    // 1. Setup OpenGL 4.6
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Assignment 5: Vertex Shader Sin Wave", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // 2. Init GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        return -1;
    }

    // --- Build Shaders ---
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --- Create Geometry (A Flat Line) ---
    // We generate a lot of points along the X axis so the wave looks smooth
    struct Vertex { float x, y, z; };
    std::vector<Vertex> vertices;

    int numPoints = 1000;
    for (int i = 0; i < numPoints; ++i) {
        // Map i from [0, 1000] to [-1.0, 1.0]
        float t = (float)i / (numPoints - 1);
        float x = t * 2.0f - 1.0f;

        // Y is 0.0 because the Vertex Shader will calculate it!
        vertices.push_back({ x, 0.0f, 0.0f });
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    sf::Clock clock;

    // --- Main Loop ---
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Send 'Time' to the shader so it animates
        float timeValue = clock.getElapsedTime().asSeconds();
        glUniform1f(glGetUniformLocation(shaderProgram, "u_Time"), timeValue);

        glBindVertexArray(VAO);

        // Draw as a Line Strip (connects the dots)
        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}