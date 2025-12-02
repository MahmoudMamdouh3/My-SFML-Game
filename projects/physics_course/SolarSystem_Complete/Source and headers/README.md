# Solar System Simulator - Mahmoud Mamdouh Mahmoud - physics course

An interactive physics-based solar system simulation built with SFML 3.0.2 and Box2D.

## 🎮 To Play:

1. Navigate to the `Executable/` folder
2. Run `My-SFML-Game.exe`
3. Enjoy the cosmic simulation!

### Controls:
- **LEFT CLICK**: Spawn a new planet that orbits around the Sun
- **O**: Toggle orbital trails on/off
- **P**: Pause/Resume simulation
- **R**: Reset solar system to initial state
- **H**: Toggle help panel

### Features:
- Realistic gravitational physics
- Multiple planets with orbital mechanics
- Beautiful sun effects with solar flares
- Customizable planet spawning
- Stunning cosmic visuals

## 🛠️ To Compile (For Developers):

### Requirements:
- **SFML 3.0.2** (automatically downloaded via CMake)
- **Box2D** (automatically downloaded via CMake)
- **CMake** (version 3.20 or higher)
- **C++17 compatible compiler** (Visual Studio 2022, GCC 9+, Clang 10+)

### Build Instructions:

1. **Clone or extract** the `Source/` folder contents
2. **Open terminal/command prompt** in the source directory
3. **Configure build:**
   ```bash
   cmake -S . -B build
   ```
4. **Build the project:**
   ```bash
   cmake --build build --config Release
   ```
5. **Run the executable:**
   ```bash
   ./build/bin/Release/solarsystem.exe
   ```


## 📁 Package Contents:

```
📁 SolarSystem_Complete/
├── 📁 Executable/        # Ready-to-play version
│   ├── My-SFML-Game.exe  # Main executable
│   └── *.dll             # Required SFML/Box2D libraries
│
├── 📁 Source and headers/           # Complete source code  # Header files (.h)
│         
│   ├── *.cpp           # Implementation files
│   ├── CMakeLists.txt  # Build configuration
│   

└── README.md           # This file
```


## 📝 Technical Details:

- **Engine**: SFML 3.0.2 (Simple and Fast Multimedia Library)
- **Physics**: Box2D (2D physics simulation)
- **Language**: C++17
- **Build System**: CMake
- **Architecture**: Modular OOP design

---

**Enjoy exploring the cosmos!** 🌌🚀✨
