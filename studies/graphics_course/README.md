Name: Mahmoud Mamdouh Mahmoud

The project is configured to automatically download and build dependencies, 
eliminating the need for manual Dependency installation. 
Each assignment demonstrates different OpenGL concepts.
# Graphics Course Assignments
This directory contains My 5 OpenGL graphics programming assignments implemented using C++, SFML, and GLAD.

## Prerequisites

### Required Software
- **CMake** (version 3.28 or higher)
- **C++ Compiler** with C++17 support
  - Visual Studio 2022 (Windows)
- **Git** (for dependency fetching)

### OpenGL Setup
- **Version**: OpenGL 4.6 Core Profile
- **Loader**: GLAD (generated for OpenGL 4.6)
- **Window**: SFML 3.0.2
- **Shaders**: GLSL 460 

	### Key Files
	- `glad.c` - OpenGL function loader implementation
	- `include/glad/glad.h` - OpenGL function declarations
	- `CMakeLists.txt` - Build configuration

## Project Structure

```
src/graphics_course/
├── README.md (this file)
├── CMakeLists.txt        # Standalone build configuration
├── 01_ColorWheel.cpp     # Rotating color wheel with OpenGL
├── 02_Sierpinski.cpp     # Sierpinski triangle fractal
├── 03_Mandelbrot.cpp     # Mandelbrot set visualization
├── 04_LineDrawer.cpp     # Interactive line drawing
├── 05_SinWave.cpp        # Sine wave animation

```
