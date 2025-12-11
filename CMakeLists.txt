cmake_minimum_required(VERSION 3.28)
project(MyGames LANGUAGES CXX C)

# 1. Global Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable "Solution Folders" (cleaner view in Visual Studio)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 2. Dependencies (FetchContent)
include(FetchContent)

# --- SFML 3.0.2 ---
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# --- Box2D 2.4.2 ---
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.2
    GIT_SHALLOW ON
)

# NEW: Disable Box2D's internal junk
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(box2d)

# 3. Add Sub-Projects
# We MUST add 'core' first because everyone else relies on it
add_subdirectory(core)
add_subdirectory(studies)
add_subdirectory(games)
# add_subdirectory(sandbox) # Uncomment this when you are ready to use the sandbox folder