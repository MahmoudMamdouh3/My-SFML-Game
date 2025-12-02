# Main Game Project
# Note: SFML is configured at the root level

add_executable(MainGame main.cpp)

target_compile_features(MainGame PRIVATE cxx_std_17)
target_include_directories(MainGame PRIVATE
    ${CMAKE_SOURCE_DIR}/shared/include
)
target_link_libraries(MainGame PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# Organize in IDE
set_target_properties(MainGame PROPERTIES FOLDER "Main Game")
