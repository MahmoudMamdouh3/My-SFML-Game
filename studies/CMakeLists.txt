# studies/CMakeLists.txt

# ====================================================
# 1. HELPER MACROS
# ====================================================

# Macro for simple single-file assignments (like Graphics)
macro(create_study_target TARGET_NAME SOURCE_FILE FOLDER_NAME)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    
    # MAGIC LINE: This one line adds SFML, Box2D, Glad, and Includes!
    target_link_libraries(${TARGET_NAME} PRIVATE Core)
    
    # Organize in Visual Studio
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
endmacro()

# ====================================================
# 2. GRAPHICS COURSE
# ====================================================
# Using our new macro to add them in one line each
create_study_target(01_ColorWheel "graphics_course/01_ColorWheel.cpp" "Studies/Graphics")
create_study_target(02_Sierpinski "graphics_course/02_Sierpinski.cpp" "Studies/Graphics")
create_study_target(03_Mandelbrot "graphics_course/03_Mandelbrot.cpp" "Studies/Graphics")
create_study_target(04_LineDrawer "graphics_course/04_LineDrawer.cpp" "Studies/Graphics")
create_study_target(05_SinWave    "graphics_course/05_SinWave.cpp"    "Studies/Graphics")

# ====================================================
# 3. PHYSICS COURSE
# ====================================================

# D. Physics Engine Tasks
add_executable(Physics_Engine_Task
    "physics_course/tasks/Main.cpp"
    "physics_course/tasks/Vector2d.cpp"
    "physics_course/tasks/Renderer.cpp"
    "physics_course/tasks/Collider.cpp"
    "physics_course/tasks/AABBCollider.cpp"
    "physics_course/tasks/SATCollider.cpp"
    "physics_course/tasks/Particle.cpp"
    "physics_course/tasks/PhysicsWorld.cpp"
)

# This allows your code to use #include "headers/..."
target_include_directories(Physics_Engine_Task PRIVATE "physics_course/tasks")

# Link SFML/Core
target_link_libraries(Physics_Engine_Task PRIVATE Core)

# Organize in Visual Studio solution explorer
set_target_properties(Physics_Engine_Task PROPERTIES FOLDER "Studies/Physics")

















# A. Solar System Lab
add_executable(Physics_Lab
    "physics_course/lab/main.cpp"
    "physics_course/lab/SolarSystem.cpp"
    "physics_course/lab/CelestialBody.cpp"
    "physics_course/lab/BackgroundManager.cpp"
    "physics_course/lab/ExplosionManager.cpp"
    "physics_course/lab/Slider.cpp"
)
target_include_directories(Physics_Lab PRIVATE "physics_course/lab/headers")
target_link_libraries(Physics_Lab PRIVATE Core)
set_target_properties(Physics_Lab PROPERTIES FOLDER "Studies/Physics")

# B. Solar System (Simple)
add_executable(Physics_SolarSystem
    "physics_course/SolarSystem/main.cpp"
    "physics_course/SolarSystem/SolarSystem.cpp"
    "physics_course/SolarSystem/CelestialBody.cpp"
    "physics_course/SolarSystem/BackgroundManager.cpp"
    "physics_course/SolarSystem/ExplosionManager.cpp"
    "physics_course/SolarSystem/Slider.cpp"
)
target_include_directories(Physics_SolarSystem PRIVATE "physics_course/SolarSystem/headers")
target_link_libraries(Physics_SolarSystem PRIVATE Core)
set_target_properties(Physics_SolarSystem PROPERTIES FOLDER "Studies/Physics")

# C. Solar System (Complete) - Handling spaces in folder name
add_executable(Physics_SolarSystem_Final
    "physics_course/SolarSystem_Complete/Source and headers/main.cpp"
    "physics_course/SolarSystem_Complete/Source and headers/SolarSystem.cpp"
    "physics_course/SolarSystem_Complete/Source and headers/CelestialBody.cpp"
    "physics_course/SolarSystem_Complete/Source and headers/BackgroundManager.cpp"
    "physics_course/SolarSystem_Complete/Source and headers/ExplosionManager.cpp"
    "physics_course/SolarSystem_Complete/Source and headers/Slider.cpp"
)
# Note: Ensure the "headers" folder is actually inside "Source and headers"
target_include_directories(Physics_SolarSystem_Final PRIVATE "physics_course/SolarSystem_Complete/Source and headers/headers")
target_link_libraries(Physics_SolarSystem_Final PRIVATE Core)
set_target_properties(Physics_SolarSystem_Final PROPERTIES FOLDER "Studies/Physics")