# Graphics Course Assignments
# Note: SFML and dependencies are configured at the root level

# Helper macro for graphics assignments
macro(add_graphics_assignment TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME}
        ${SOURCE_FILE}
        ${CMAKE_SOURCE_DIR}/shared/src/glad.c
    )
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/include
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
    )
    # Organize in IDE
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Graphics Course")
endmacro()

# Graphics Course Assignments
add_graphics_assignment(01_ColorWheel 01_ColorWheel.cpp)
add_graphics_assignment(02_Sierpinski 02_Sierpinski.cpp)
add_graphics_assignment(03_Mandelbrot 03_Mandelbrot.cpp)
add_graphics_assignment(04_LineDrawer 04_LineDrawer.cpp)
add_graphics_assignment(05_SinWave 05_SinWave.cpp)
