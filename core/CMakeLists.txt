project(CoreLibrary)

# --------------------------------------------------------
# 1. Create the Library
# --------------------------------------------------------
# This compiles your shared code into a static library
add_library(Core STATIC
    src/glad.c
    # In the future, add your own shared .cpp files here:
    # src/MathUtils.cpp
    # src/Logger.cpp
)

# --------------------------------------------------------
# 2. Define Include Paths
# --------------------------------------------------------
# PUBLIC means: "Anyone who uses Core can also see these headers"
target_include_directories(Core PUBLIC 
    include
)

# --------------------------------------------------------
# 3. Link Dependencies (The "Kitchen Sink" Approach)
# --------------------------------------------------------
# We link these PUBLICly so that any game linking 'Core'
# automatically gets access to SFML, Box2D, and OpenGL symbols.
target_link_libraries(Core PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    box2d
)

# --------------------------------------------------------
# 4. IDE Organization
# --------------------------------------------------------
set_target_properties(Core PROPERTIES FOLDER "Engine Core")