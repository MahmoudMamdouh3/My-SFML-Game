# Physics Course Projects
# Note: SFML and Box2D are configured at the root level

# Helper macro for physics projects
macro(add_physics_project TARGET_NAME)
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/include
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        box2d
    )
    # Organize in IDE
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Physics Course")
endmacro()

# Solar System Lab Project
add_executable(solarsystem_lab
    lab/main.cpp
    lab/SolarSystem.cpp
    lab/CelestialBody.cpp
    lab/BackgroundManager.cpp
    lab/ExplosionManager.cpp
    lab/Slider.cpp
)
target_include_directories(solarsystem_lab PRIVATE lab/headers)
add_physics_project(solarsystem_lab)

# Solar System Project (simplified version)
add_executable(solarsystem
    SolarSystem/main.cpp
    SolarSystem/SolarSystem.cpp
    SolarSystem/CelestialBody.cpp
    SolarSystem/BackgroundManager.cpp
    SolarSystem/ExplosionManager.cpp
    SolarSystem/Slider.cpp
)
target_include_directories(solarsystem PRIVATE SolarSystem/headers)
add_physics_project(solarsystem)

# Solar System Complete Project
add_executable(solarsystem_complete
    SolarSystem_Complete/Source\ and\ headers/main.cpp
    SolarSystem_Complete/Source\ and\ headers/SolarSystem.cpp
    SolarSystem_Complete/Source\ and\ headers/CelestialBody.cpp
    SolarSystem_Complete/Source\ and\ headers/BackgroundManager.cpp
    SolarSystem_Complete/Source\ and\ headers/ExplosionManager.cpp
    SolarSystem_Complete/Source\ and\ headers/Slider.cpp
)
target_include_directories(solarsystem_complete PRIVATE "SolarSystem_Complete/Source and headers/headers")
add_physics_project(solarsystem_complete)
