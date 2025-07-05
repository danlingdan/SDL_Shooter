# SDL Shooter

A 2D space shooter game built with SDL2 where you control a spaceship to defeat incoming enemy ships.

## Game Description

SDL Shooter is a classic-style 2D vertical shooter where you pilot a spaceship against waves of enemy ships. Shoot down enemies, avoid their bullets, and collect power-ups to survive as long as possible and achieve a high score.

## Features

- Fast-paced arcade-style gameplay
- Enemy ships with different attack patterns
- Score tracking system
- Health system with visual indicators
- Power-up/item collection system
- Sound effects and background music
- Explosion animations

## Controls

- **W, A, S, D** - Move the spaceship (up, left, down, right)
- **J** - Fire weapon
- **ESC** - Pause/Menu

## Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## Building the Game

This game uses C++14 and SDL2 libraries. Make sure you have the following installed:

1. A C++14 compatible compiler (MSVC, GCC, Clang)
2. SDL2 development libraries
3. SDL2_image, SDL2_mixer, and SDL2_ttf development libraries

### Building with Visual Studio

1. Open the solution file in Visual Studio 2022
2. Make sure you have set up the necessary SDL2 dependencies
3. Build the solution in Release or Debug mode
4. Run the game

### Dependencies with vcpkg

You can use vcpkg to easily install the required dependencies:

```
vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
```

## Game Structure

- **Title Scene**: Game start screen
- **Main Scene**: Main gameplay
- **End Scene**: Game over screen with score display

## Credits

This game uses SDL2 (Simple DirectMedia Layer) and its extension libraries for graphics, sound, and input handling.

---
