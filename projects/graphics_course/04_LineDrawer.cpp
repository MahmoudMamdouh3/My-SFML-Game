#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <optional>

struct Point { float x, y, z; };

const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    void main() { gl_Position = vec4(aPos, 1.0); }
)";

const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    uniform vec3 u_Color;
    void main() { FragColor = vec4(u_Color, 1.0); }
)";

Point mouseToNDC(const sf::Window& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u size = window.getSize();
    float ndcX = (float)mousePos.x / (float)size.x * 2.0f - 1.0f;
    float ndcY = -((float)mousePos.y / (float)size.y * 2.0f - 1.0f);
    return { ndcX, ndcY, 0.0f };
}

void checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
        }
    }
}

int main() {
    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;


    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Line Drawer", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    window.setActive(true);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) return -1;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 200000 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    std::vector<Point> allPoints;
    std::vector<int> strokeStarts;
    std::vector<int> strokeCounts;
    bool isDrawing = false;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    isDrawing = true;
                    strokeStarts.push_back(static_cast<int>(allPoints.size()));
                    strokeCounts.push_back(0);
                }
            }

            if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRelease->button == sf::Mouse::Button::Left)
                    isDrawing = false;
            }
        }

        if (isDrawing && !strokeCounts.empty()) {
            allPoints.push_back(mouseToNDC(window));
            strokeCounts.back()++;

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, allPoints.size() * sizeof(Point), allPoints.data());
        }

        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glUniform3f(glGetUniformLocation(shaderProgram, "u_Color"), 0.f, 0.f, 1.f);
        glLineWidth(3.f);

        for (size_t i = 0; i < strokeStarts.size(); i++) {
            if (strokeCounts[i] > 0) {
                glDrawArrays(GL_LINE_STRIP, strokeStarts[i], strokeCounts[i]);
            }
        }

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}