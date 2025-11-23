cmake_minimum_required(VERSION 3.28)
project(GraphicsCourse LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# --- Fetch SFML 3 (Latest) ---
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# --- Fetch Box2D ---
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.2
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(box2d)

# --- Helper Macro ---
macro(add_assignment_project TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME}
        ${SOURCE_FILE}
        src/glad.c
    )
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            SFML::Graphics
            SFML::Window
            SFML::System
            SFML::Audio
            SFML::Network
            box2d
    )
endmacro()

# ====================================================
# YOUR ASSIGNMENTS
# ====================================================

# Register the old main.cpp as its own executable called "MainGame"
add_assignment_project(MainGame "src/main.cpp")

# Assignment 1: Color Wheel
add_assignment_project(01_ColorWheel "src/graphics_course/01_ColorWheel.cpp")

# Assignment 2: Sierpinski
add_assignment_project(02_Sierpinski "src/graphics_course/02_Sierpinski.cpp")

# Assignment 3: Mandelbrot
add_assignment_project(03_Mandelbrot "src/graphics_course/03_Mandelbrot.cpp")

# Assignment 4: Line Drawer
add_assignment_project(04_LineDrawer "src/graphics_course/04_LineDrawer.cpp")

# Assignment 5: Sin Wave
add_assignment_project(05_SinWave "src/graphics_course/05_SinWave.cpp")