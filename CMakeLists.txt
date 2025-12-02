cmake_minimum_required(VERSION 3.28)
project(MyGames LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable folders for IDE organization
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)

# Try to find SFML and Box2D on the system first
find_package(SFML 3.0.2 QUIET)
find_package(box2d QUIET)

# If not found, fetch them
if(NOT SFML_FOUND)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    FetchContent_MakeAvailable(SFML)
endif()

if(NOT box2d_FOUND)
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.2
        GIT_SHALLOW ON
        SYSTEM
    )
    FetchContent_MakeAvailable(box2d)
endif()

# Make shared resources available to all projects
include_directories(${CMAKE_SOURCE_DIR}/shared/include)

# Add project subdirectories
add_subdirectory(projects/graphics_course)
add_subdirectory(projects/physics_course)
add_subdirectory(projects/main_game)

# DSA course is currently empty, uncomment when ready
# add_subdirectory(projects/DSA_OOP_Course)