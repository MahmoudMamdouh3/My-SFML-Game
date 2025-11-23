#include <glad/glad.h>  // Include BEFORE SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// --- Vertex Shader ---
const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// --- Fragment Shader ---
// We use a 'uniform' so we can change color (Red for dot, Blue for line)
const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    uniform vec3 u_Color;
    void main() {
        FragColor = vec4(u_Color, 1.0);
    }
)";

// --- Helper: Convert Mouse (Pixels) to OpenGL (NDC -1 to 1) ---
struct Point { float x, y, z; };

Point mouseToNDC(const sf::Window& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u size = window.getSize();

    // Map X from [0, Width] to [-1, 1]
    float ndcX = (float)mousePos.x / (float)size.x * 2.0f - 1.0f;

    // Map Y from [0, Height] to [1, -1] (Invert Y)
    float ndcY = -((float)mousePos.y / (float)size.y * 2.0f - 1.0f);

    return { ndcX, ndcY, 0.0f };
}

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
    // 1. Setup OpenGL
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Assignment 4: Bezier Line Drawer", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // 2. Init GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        return -1;
    }

    // --- Shaders ---
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

    // --- Geometry Data ---
    // Fixed Start and End points
    Point p0 = { -0.8f, -0.5f, 0.0f }; // Left
    Point p2 = { 0.8f, -0.5f, 0.0f }; // Right
    Point p1 = { 0.0f,  0.8f, 0.0f }; // Control Point (Starts Top Center)

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // We allocate space for 100 points, but don't fill it yet. 
    // Usage is DYNAMIC_DRAW because we update it every frame.
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(Point), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    // Make points look like circles
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- Update Logic ---
        // 1. Update Control Point (P1) to follow Mouse
        p1 = mouseToNDC(window);

        // 2. Calculate Bezier Curve Points (Quadratic)
        // Formula: B(t) = (1-t)^2 * P0 + 2(1-t)t * P1 + t^2 * P2
        std::vector<Point> curvePoints;
        int segments = 50;

        for (int i = 0; i <= segments; ++i) {
            float t = (float)i / (float)segments;
            float u = 1.0f - t;
            float tt = t * t;
            float uu = u * u;

            float x = (uu * p0.x) + (2 * u * t * p1.x) + (tt * p2.x);
            float y = (uu * p0.y) + (2 * u * t * p1.y) + (tt * p2.y);

            curvePoints.push_back({ x, y, 0.0f });
        }

        // 3. Add the Control Point at the end so we can draw it as a dot
        curvePoints.push_back(p1);

        // 4. Upload new data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, curvePoints.size() * sizeof(Point), curvePoints.data());

        // --- Render ---
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White Background
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw Curve (Blue)
        // Use all points EXCEPT the last one (which is the control dot)
        glLineWidth(3.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "u_Color"), 0.0f, 0.0f, 1.0f); // Blue
        glDrawArrays(GL_LINE_STRIP, 0, segments + 1);

        // Draw Control Point (Red Dot)
        // Draw ONLY the last point
        glPointSize(15.0f); // Make it big
        glUniform3f(glGetUniformLocation(shaderProgram, "u_Color"), 1.0f, 0.0f, 0.0f); // Red
        glDrawArrays(GL_POINTS, segments + 1, 1);

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}