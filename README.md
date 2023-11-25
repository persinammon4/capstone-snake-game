# Capstone Snake Game

The original project had a fully functional Snake game implemented. A Snake game has a keyboard controlled snake that's constantly moving.
When it eats a food (by running over it), it grows in length by one block while still moving. The original implementation infinitely runs until the snake 
runs into itself.

Original implementation:
- A `main` function handling creating objects of the three main steps of the game loop (`Controller`, `Game`, `Renderer`). The `Game::Game` constructor creates a `Snake` object, while `Game::Run` starts the game loop.
- A `Game::Update` method that takes care of snake movement, handling the snake eating food, basic movement in response to user input.
- Score updated regularly.
- The `Snake` stores float coordinates of the head, and a vector of points (int cell coordinates) as the body. The head has float
coordinates to suggest speed as a gradient, versus blockily moving from point to point. Location updates based on the snake's speed
are all handled. The `Snake::Update` method both handles updating the head and the body.
- Most of the SDL usage is already written.

The project spec asked to create features to the original game which used concepts discussed through the course. 

Added features:
- Create a GUI with speed (frames per second) toggling, leaderboard option, original mode, obstacle mode, computer snake mode.
- Leaderboard is read and written from a `*.txt` file.
- New `Obstacle` class with inheritance (fixed, moving).
- New snake controlled by A* algorithm, which will lead to a loss if run into it or
automatic win/rankings multiplier boost if eat the snake by hitting the last box of it.
- End print message with score, size, and leaderboard ranking (two leaderboards - respective mode and the general leaderboard).

<img src="snake_game.gif"/>

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

Or: 

1. Download the executable `build/SnakeGame` and run it through `./SnakeGame` in the same directory.

## Credits

Udacity team credited [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow thread.

## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
