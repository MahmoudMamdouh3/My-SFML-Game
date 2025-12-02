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

### Dependencies (Automatically Downloaded)
- **SFML 3.0.2** - Window and graphics library
- **GLAD** - OpenGL loader (included as `glad.c` and `glad.h`)
- **Box2D 2.4.2** - Physics engine (included but not used in graphics assignments)

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
└── Executables/          # Pre-built executables (click to run)
    ├── 01_ColorWheel.exe
    ├── 02_Sierpinski.exe
    ├── 03_Mandelbrot.exe
    ├── 04_LineDrawer.exe
    └── 05_SinWave.exe
```

## Quick Start (Pre-built Executables)

For immediate testing without building, use the pre-built executables in the `Executables/` folder:

```bash
# Simply double-click these files in Windows Explorer:
# - Executables/01_ColorWheel.exe
# - Executables/02_Sierpinski.exe
# - Executables/03_Mandelbrot.exe
# - Executables/04_LineDrawer.exe
# - Executables/05_SinWave.exe
```

**Note:** These executables are pre-built and ready to run on compatible Windows systems.

*To create the Executables folder with built binaries:*
```bash
# After building the main project, copy executables:
cp ../../build/bin/Debug/01_ColorWheel.exe Executables/
cp ../../build/bin/Debug/02_Sierpinski.exe Executables/
cp ../../build/bin/Debug/03_Mandelbrot.exe Executables/
cp ../../build/bin/Debug/04_LineDrawer.exe Executables/
cp ../../build/bin/Debug/05_SinWave.exe Executables/
```

## Build Instructions

### Opening in Visual Studio (Recommended)
1. Open Visual Studio
2. Select "Open a local folder"
3. Navigate to and select the `src/graphics_course/` folder
4. Visual Studio will automatically detect the CMakeLists.txt and configure the project
5. Build and run the assignments from within Visual Studio

### Standalone CMake Build
If you prefer manual CMake commands:
```bash
cd src/graphics_course
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

**Note:** The standalone CMakeLists.txt expects `glad.c` and `glad.h` to be available in the parent directory structure.

### Manual Build
**Note:** The first CMake configuration may take several minutes as it downloads and builds SFML 3.0.2 from source.

### Build System
- **Generator**: Visual Studio 17 2022 (Windows) or Unix Makefiles
- **Language**: C++17
- **Output**: Executables in `build/bin/`

### Common Issues

**1. CMake Configuration Takes Too Long**
- This is normal - SFML is being downloaded and built from source
- Subsequent builds will be much faster

**2. "Unresolved External Symbol" Errors**
- Make sure `glad.c` is included in the build (it should be automatically)
- Verify CMake configuration completed successfully

**3. OpenGL Context Creation Fails**
- Ensure your graphics drivers support OpenGL 4.6
- Try updating graphics drivers

**4. Window Doesn't Appear**
- Check that executables are in the correct output directory
- Run from command line to see error messages

### Build Clean
If you encounter build issues:
```bash
# Clean and rebuild
rm -rf build/
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

