#include <glad/glad.h>       // Must be included BEFORE SFML headers
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// --- Shader Sources (Modern OpenGL 4.6) ---
const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 ourColor;
    
    uniform float rotationAngle; // We will change this from C++

    void main() {
        // Simple 2D Rotation Matrix
        float s = sin(rotationAngle);
        float c = cos(rotationAngle);
        mat2 rot = mat2(c, -s, s, c);
        
        vec2 rotatedPos = rot * aPos.xy;
        gl_Position = vec4(rotatedPos, aPos.z, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    in vec3 ourColor;
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";

// Helper to check shader compilation errors
void checkShaderCompile(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
    }
}

int main() {
    // 1. Setup OpenGL 4.6 Settings
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    // 2. Create Window (SFML 3 Style)
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Assignment 1: Color Wheel", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // 3. Init GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --- Build and Compile Shaders ---
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --- Create Color Wheel Geometry ---
    // Center point + vertices around the circle
    struct Vertex {
        float x, y, z;
        float r, g, b;
    };

    std::vector<Vertex> vertices;
    // Center Vertex (White or Black)
    vertices.push_back({ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f });

    int segments = 12; // As seen in your slide
    float radius = 0.5f;
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * 3.14159f;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        // Color changes based on angle
        float r = (sin(angle) + 1.0f) * 0.5f;
        float g = (cos(angle) + 1.0f) * 0.5f;
        float b = 0.5f;

        vertices.push_back({ x, y, 0.0f, r, g, b });
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- Main Loop ---
    sf::Clock clock;

    while (window.isOpen()) {
        // SFML 3 Event Handling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Update Rotation Uniform
        float timeValue = clock.getElapsedTime().asSeconds();
        int rotationLoc = glGetUniformLocation(shaderProgram, "rotationAngle");
        glUniform1f(rotationLoc, timeValue); // Pass time as angle

        glBindVertexArray(VAO);
        // Draw Triangle Fan (Center connected to all outer points)
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}