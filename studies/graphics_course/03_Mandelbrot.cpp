#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;

    uniform vec2 u_resolution;
    uniform float u_zoom;
    uniform vec2 u_center;

    void main() {
        // 1. Normalize coordinates to -1.0 to 1.0 range, correcting for aspect ratio
        vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / u_resolution.y;
        
        // 2. Apply Zoom and Center (Map screen to Complex Plane)
        // Default view: Center (-0.5, 0.0), Zoom 1.0
        vec2 c = (uv * 2.5 / u_zoom) + u_center; 

        // 3. Mandelbrot Iteration: z = z^2 + c
        vec2 z = vec2(0.0);
        int maxIter = 100;
        int iter = 0;
        
        for(int i = 0; i < maxIter; i++) {
            // z^2 = (x + iy)^2 = x^2 - y^2 + 2ixy
            float x = (z.x * z.x - z.y * z.y) + c.x;
            float y = (2.0 * z.x * z.y) + c.y;
            z = vec2(x, y);
            
            // Check if escaped (magnitude > 2.0, or magnitude^2 > 4.0)
            if(dot(z, z) > 4.0) {
                iter = i;
                break;
            }
            iter = maxIter;
        }

        // 4. Coloring
        if (iter == maxIter) {
            // Inside the set -> Black
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        } else {
            // Outside the set -> White/Grayscale gradient based on iteration
            float t = float(iter) / float(maxIter);
            // You can make this colorful by messing with these channels
            FragColor = vec4(t, t, t, 1.0); 
        }
    }
)";

void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

int main() {
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Assignment 3: Mandelbrot", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

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

    float vertices[] = {
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float zoom = 1.0f;
    float centerX = -0.75f;
    float centerY = 0.0f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Equal) zoom *= 1.1f;
                if (keyPressed->code == sf::Keyboard::Key::Hyphen) zoom /= 1.1f;
                if (keyPressed->code == sf::Keyboard::Key::Right) centerX += 0.1f / zoom;
                if (keyPressed->code == sf::Keyboard::Key::Left) centerX -= 0.1f / zoom;
                if (keyPressed->code == sf::Keyboard::Key::Up) centerY += 0.1f / zoom;
                if (keyPressed->code == sf::Keyboard::Key::Down) centerY -= 0.1f / zoom;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        sf::Vector2u size = window.getSize();
        glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), (float)size.x, (float)size.y);
        glUniform1f(glGetUniformLocation(shaderProgram, "u_zoom"), zoom);
        glUniform2f(glGetUniformLocation(shaderProgram, "u_center"), centerX, centerY);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    return 0;
}