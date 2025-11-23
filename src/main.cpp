#include <glad/glad.h> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// =============================================================
// SHADER SOURCE (Neon Ripple Effect)
// =============================================================
const char* vertexShaderSrc = R"(
    #version 460 core
    layout (location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
)";

const char* fragmentShaderSrc = R"(
    #version 460 core
    out vec4 FragColor;
    uniform vec2 u_resolution; 
    uniform float u_time;      

    // Cosine based palette function
    vec3 palette( float t ) {
        vec3 a = vec3(0.5, 0.5, 0.5);
        vec3 b = vec3(0.5, 0.5, 0.5);
        vec3 c = vec3(1.0, 1.0, 1.0);
        vec3 d = vec3(0.263, 0.416, 0.557);
        return a + b * cos( 6.28318 * (c * t + d) );
    }

    void main() {
        vec2 uv = (gl_FragCoord.xy * 2.0 - u_resolution.xy) / u_resolution.y;
        vec2 uv0 = uv;
        vec3 finalColor = vec3(0.0);

        for (float i = 0.0; i < 4.0; i++) {
            uv = fract(uv * 1.5) - 0.5;
            float d = length(uv) * exp(-length(uv0));
            vec3 col = palette(length(uv0) + i * 0.4 + u_time * 0.4);
            d = sin(d * 8.0 + u_time) / 8.0;
            d = abs(d);
            d = pow(0.01 / d, 1.2);
            finalColor += col * d;
        }
        FragColor = vec4(finalColor, 1.0);
    }
)";

// --- Shader Helper ---
unsigned int createShader(const char* vSrc, const char* fSrc) {
    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vSrc, NULL);
    glCompileShader(v);

    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fSrc, NULL);
    glCompileShader(f);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

int main() {
    // 1. SFML Window Setup
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3 + Neon Shader", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // 2. GLAD Init
    window.setActive(true);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // 3. OpenGL Data (Full Screen Quad)
    float vertices[] = {
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int shaderProgram = createShader(vertexShaderSrc, fragmentShaderSrc);
    int uTimeLoc = glGetUniformLocation(shaderProgram, "u_time");
    int uResLoc = glGetUniformLocation(shaderProgram, "u_resolution");

    // 4. SFML Text Setup (Optional)
    sf::Font font;
    // Attempt to load a font. If this fails, the app still runs, just no text.
    // Ensure you have a .ttf file (like arial.ttf) in your build folder if you want text.
    bool fontLoaded = font.openFromFile("arial.ttf");
    sf::Text text(font, "SFML 3 + OpenGL 4.6", 24);
    text.setPosition({ 10, 10 });

    sf::Clock clock;

    // 5. Game Loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        // --- DRAW OPENGL (Background) ---
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Update Shader Uniforms
        glUniform1f(uTimeLoc, clock.getElapsedTime().asSeconds());
        sf::Vector2u size = window.getSize();
        glUniform2f(uResLoc, (float)size.x, (float)size.y);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // -------------------------------------------------------------
        // [IMPORTANT] UNBIND EVERYTHING BEFORE SFML DRAWS
        // If you miss this, SFML will crash or flicker!
        // -------------------------------------------------------------
        glBindVertexArray(0);
        glUseProgram(0);

        // --- DRAW SFML (Foreground) ---
        if (fontLoaded) {
            window.pushGLStates(); // Save GL state
            window.draw(text);     // Draw SFML
            window.popGLStates();  // Restore GL state
        }

        window.display();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}