#include <glad/glad.h>  // Must be included BEFORE SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

// --- Shaders ---
const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// Blue color to match your slide example
const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
    }
)";

// --- Helper Structs ---
struct Point {
    float x, y, z;
};

// --- Recursive Function to Generate Geometry ---
void divideTriangle(const Point& a, const Point& b, const Point& c, int count, std::vector<Point>& vertices) {
    if (count == 0) {
        // Base case: Push the actual triangle vertices
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    }
    else {
        // Calculate midpoints
        Point ab = { (a.x + b.x) / 2.0f, (a.y + b.y) / 2.0f, 0.0f };
        Point ac = { (a.x + c.x) / 2.0f, (a.y + c.y) / 2.0f, 0.0f };
        Point bc = { (b.x + c.x) / 2.0f, (b.y + c.y) / 2.0f, 0.0f };

        // Subdivide into 3 smaller triangles (Top, Left, Right)
        // Note: We skip the middle triangle to create the "hole"
        divideTriangle(a, ab, ac, count - 1, vertices);
        divideTriangle(ab, b, bc, count - 1, vertices);
        divideTriangle(bc, c, ac, count - 1, vertices);
    }
}

// Helper to check for shader errors
void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

int main() {
    // 1. Setup OpenGL 4.6 Core
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.depthBits = 24;

    // 2. Create Window (SFML 3 Style)
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Assignment 2: Sierpinski Triangle", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // 3. Init GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --- Shader Setup ---
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

    // --- Generate Sierpinski Geometry ---
    std::vector<Point> vertices;

    // Initial Triangle Points (NDC Coordinates: -1 to 1)
    Point p1 = { 0.0f,  1.0f, 0.0f };   // Top Center
    Point p2 = { -1.0f, -1.0f, 0.0f };  // Bottom Left
    Point p3 = { 1.0f, -1.0f, 0.0f };   // Bottom Right

    // Recursion Depth (Try changing this to 7 or 8 for more detail!)
    int depth = 6;
    divideTriangle(p1, p2, p3, depth, vertices);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Set Drawing Mode to "Lines" ---
    // This draws the outline of the triangles like in your slide
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // --- Main Loop ---
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        // White Background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw all generated triangles
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}