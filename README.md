<div align="center">

# Tower Defense — The NullTerminators

**A 2D Grid-Based Tower Defense Game Engine & Map Editor built from scratch in Modern C++23 and SDL3.**

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![SDL3](https://img.shields.io/badge/SDL-3.0-17355A?style=for-the-badge&logo=libsdl&logoColor=white)](https://www.libsdl.org/)
[![CMake](https://img.shields.io/badge/CMake-3.8+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![Download Release](https://img.shields.io/badge/Download-v1.0.0_Release-success?style=for-the-badge&logo=github)](https://github.com/NathanGrenier/COMP-345/releases/download/v1.0.0/COMP-345.zip)

<br />

<img src="static/Gameplay - COMP-345.gif" alt="Gameplay Demo" width="750" />

<br />

[📥 **Download Playable Game (Windows .zip)**](https://github.com/NathanGrenier/COMP-345/releases/download/v1.0.0/COMP-345.zip) · [📂 **Explore Source Code**](./src)

</div>

## Overview

Developed for **COMP-345 (Advanced Program Design with C++)**, this project is a complete 2D Tower Defense game and custom level editor engineered without a commercial game engine. 

It showcases **software architecture, Gang of Four (GoF) design patterns, real-time pathfinding algorithms, and resource management** in modern C++23 using the **SDL3** multimedia library.

## Key Gameplay Features

* **3 Distinct Tower Archetypes:** Deploy **Standard**, **Rapid Fire** (burst-fire with cooldown mechanics), and **Cannon** (high-impact) towers. Each has multi-tier stat upgrades, dynamic sprite evolution, and 70% sell-back value.
* **Dynamic Targeting AI:** Switch individual tower targeting priorities on the fly between *Nearest to Exit*, *Nearest to Tower*, *Strongest*, and *Weakest*.
* **Drag-and-Drop Elemental Powerups:** Defeated enemies have a chance to drop **Fire** and **Ice** powerups. Dragging them onto towers dynamically equips them with **Damage-over-Time (Burn)** or **Movement Slow** effects.
* **20 Escalating Waves & 3 Enemy Types:** Defend against **Slimes** (Balanced), **Bees** (Fast), and **Ogres** (Tanks) that scale in HP, speed, and gold rewards across 20 waves (plus an Endless Mode implementation).
* **Full-Featured Custom Map Editor:** Create, rename, resize (8x8 to 20x20 grids), and edit custom maps. Includes real-time **BFS Flow-Field path validation** to ensure enemies always have a valid route from spawner to exit before saving to JSON.
* **Polished Audio & Visuals:** Multi-layered animated parallax space backgrounds, directional sprite animations, health bars, timestamped game event logging, and an Options Menu with independent volume sliders for Music, UI, and Game SFX.

## 📸 Screenshots

<div align="center">
  <table>
    <tr>
      <td align="center"><b>Title Screen</b></td>
      <td align="center"><b>Main Menu</b></td>
    </tr>
    <tr>
      <td><img src="static/Title Screen - COMP 345.png" alt="Title Screen" width="400"/></td>
      <td><img src="static/Main Menu - COMP-345.png" alt="Main Menu" width="400"/></td>
    </tr>
    <tr>
      <td align="center"><b>Map Selection & Presets</b></td>
      <td align="center"><b>Custom Map Editor (Flow-Field View)</b></td>
    </tr>
    <tr>
      <td><img src="static/Map Selection - COMP-345.png" alt="Map Selection" width="400"/></td>
      <td><img src="static/Map Editor - COMP-345.png" alt="Map Editor" width="400"/></td>
    </tr>
  </table>
</div>

## Software Architecture & Design Patterns

A core objective of this project is demonstrating clean, decoupled, and extensible object-oriented architecture:

| Pattern / System           | Implementation in Codebase                                                                        | Purpose & Benefit                                                                                                                                               |
| :------------------------- | :------------------------------------------------------------------------------------------------ | :-------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Strategy Pattern**       | `TowerStrategy` (`TargetNearExit`, `TargetNearTower`, `TargetStrongest`, `TargetWeakest`)         | Decouples tower targeting algorithms from the `Tower` class, allowing players to hot-swap AI behavior at runtime.                                               |
| **Decorator Pattern**      | `TowerDecorator` (`BurningDecorator`, `SlowingDecorator`)                                         | Dynamically wraps base `Tower` instances at runtime when players drag-and-drop Fire or Ice powerups, adding status effects without subclass explosion.          |
| **Observer Pattern**       | `Observable` / `Observer` (`TowerObserver`, `CritterObserver`, `FlowFieldObserver`)               | Powers reactive UI inspector panels (`DetailAttributeDisplay`) and notifies entities when tower/critter stats or map flow-fields change.                        |
| **Factory Pattern**        | `CritterFactory`                                                                                  | Centralizes instantiation and wave-level stat scaling for `NormalCritter`, `FastCritter`, and `TankCritter`.                                                    |
| **State Pattern**          | `GameState` (`Intro`, `Title`, `MapSelect`, `MapEditor`, `MainGame`, `Options`, `Win`/`GameOver`) | Manages screen lifecycles (`enter`, `exit`, `handleEvent`, `update`, `render`) with smooth alpha-fade transitions.                                              |
| **Singleton & Caching**    | `TextureManager`, `AudioManager`                                                                  | Prevents redundant disk I/O and memory leaks by caching `SDL_Texture*`, `Mix_Chunk*`, and `Mix_Music*` assets across states.                                    |
| **Flow-Field Pathfinding** | `Map::calculateFlowField()`                                                                       | Uses Breadth-First Search (BFS) distance mapping from the target cell to generate a directional vector field for $O(1)$ critter navigation and path validation. |

## Tech Stack

* **Language:** C++23
* **Graphics, Windowing & Audio:** SDL3, SDL3_image, SDL3_ttf, SDL3_mixer
* **Serialization:** `nlohmann/json` (Map presets, custom maps, and win-screen quotes)
* **Build System:** CMake (Modular static library architecture: `states`, `towers`, `critter`, `map`, `ui`, `util`)
* **Documentation:** Doxygen

## Getting Started

### Option 1: Play Immediately (No Build Required)
1. Download the latest pre-compiled release: **[COMP-345 v1.0.0 (.zip)](https://github.com/NathanGrenier/COMP-345/releases/download/v1.0.0/COMP-345.zip)**
2. Extract the `.zip` archive.
3. Run `COMP-345.exe`.

### Option 2: Build from Source (Windows)
**Prerequisites:** CMake (3.8+), a C++23 compatible compiler (MSVC / Visual Studio 2022 recommended), and Git. *(All third-party dependencies like SDL3 and `nlohmann/json` are vendored in the repository).*

**Automated Build (Batch Script):**
```cmd
git clone [https://github.com/NathanGrenier/COMP-345.git](https://github.com/NathanGrenier/COMP-345.git)
cd COMP-345
.\install.bat
```
*(Flags available: `/skipstart` to build without launching, `/verbose` for detailed output, `/clean-pb` to clean intermediate build files).*

**Manual CMake Build:**
```DOS
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## Project Structure

```
COMP-345/
├── CMakeLists.txt          # Root CMake configuration & asset copy targets
├── install.bat             # One-click Windows build & launch script
└── src/
    ├── main.cpp            # Entry point, 60 FPS game loop, & state transitions
    ├── assets/             # Sprites, audio (.wav), fonts, & JSON map presets
    ├── include/            # Header files (.h) mirroring module directories
    ├── states/             # GameState implementations (Menu, Game, Editor, etc.)
    ├── towers/             # Tower classes, targeting Strategies, Decorators, & Powerups
    ├── critter/            # Critter types, wave spawning logic, & CritterFactory
    ├── map/                # Grid representation, JSON I/O, & BFS Flow-Field pathfinding
    ├── ui/                 # Custom SDL3 UI framework (Buttons, Sliders, Parallax, Observers)
    └── util/               # Singletons (TextureManager, AudioManager), Timer, & Vector2D
```