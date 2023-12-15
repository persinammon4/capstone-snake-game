# Capstone Snake Game

The Snake game is a game world where a keyboard controlled snake attempts to eat food. Every time it eats food it grows longer one by one block, but if it turns in on itself and hits another part of its body it dies. The original implementation set up a basic game loop including a Snake class, SDL library usage, and rendering of abstract objects. It runs infinitely unless the snake dies.

The extended implementation has the following features:
- Multiple Game modes: new snake controlled by A* algorithm, with its own collision rules (user snake death if run into it, will not run into obstacles or use wraparound) and a score boost if the new snake is killed by the user snake hitting its last box (essentially eating it). Second mode is to insert obstacles into the grid in randomly selected locations.
- Leaderboard with scores for all previous attempts written to `*.txt` files. I felt very uncomfortable leaving a plaintext file with data public on GitHub, so I implemented encryption with a symmetric key. Encryption is a mandatory feature. Used [Crypto++](https://github.com/weidai11/cryptopp).
- Create a GUI with speed (frames per second) toggling, leaderboard option, original mode, obstacle mode, computer snake mode. Used [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear/) to do so.

The project has in-line comments with reasoning behind implementation decisions. Details further down the README.

## How to Run

1. Download `build/SnakeGame` and run using `./SnakeGame` from Unix command line in same directory.

Or:

1. Create own executable by cloning repo.
2. If no build directory, create one `mkdir build`.
3. Compile `cd build && cmake .. && make`.
4. Run `./SnakeGame`


## Testing

This is a freeform game project, so human input and observation is required for testing (not as simple as automatically running a test suite
from command line). Recording scores and gameplay logic are both based on human input. Reading and writing files can be tested with unit tests,
but it was faster to visually inspect (this being a personal project, not professional).

1. In `main.cpp`, pass in the two game mode parameters that you want to test.
2. To add a new environment (of obstacles and whether to have an AI controlled competitor snake),
go to `scenario.h`, write a new `GameEnv[x]` where x is an unused positive integer, and change the order
of the method names in the vector to guarantee your function will be used to populate the game. Include
an invariant to ensure the correct game mode is set.
3. Comment out the GUI in `main.cpp` if necessary.
4. Build and run.

## Notes

- Possible extensions of this project:
    - Implement the ability to have multiple computer controlled snakes initialized in random locations (still all run by A*). Allow
      user to toggle how many computer controlled competitor snakes there will be.
    - For multiple computer controlled snakes, use multi-threading to run A* for each snake.
    - Think of other ways to introduce concurrency to complete tasks in parallel and speed up the game or increase the game's possible throughput.
    - Have the computer controlled snake chase the user controlled snake. :0
    - Have a way to implement an overall leaderboard being written to from multiple games run on different computers.
    - Create new scenarios or types of obstacles.
    - Add changing colors of obstacles.



- Never ever (ever ever) roll your own encryption (like Geeks4Geeks and other articles/short courses were suggesting):
https://medium.com/@jmayuresh25/create-a-simple-file-encryption-system-in-c-e3726e0f265b
Although, with the character based encryption, if you encrypt the whitespaces by introducing a simple shift in characters by one number, surely that's a way to crack the cipher. How I could know that - I read that Caesar cipher was cracked because the letter "e" in English plaintext is very frequent.

- This project was amenable to being solved quickly, because of the separation of concerns already built in (e.g. Renderer only renders a snapshot of abstract objects, imports go in header files, leaderboard is a file writer independent of other parts of the game, snake collision logic is all in one area, abstract object movement is handled inside of the object itself through the Update method, controller just needs directions fed into it allowing for A* to only focus on returning directions instead of handling updating the points themselves, GUI can be the last to be implemented).

- Figured out how to make flashing colored items! Have two `SDL_SetRenderDrawColor`s next to each other and it will effectively be a strobe light!
- `Renderer` and `Controller` don't own any of the class representations e.g. Snake, Obstacle. They just take in references or 
copies and either draw out a renderering from a `const` object or mutate values within the object. The food is 
implemented as an `SDL_Point` and not a class, which is perfect, but for some reason it's being passed in as a reference to 
the `Renderer` instead of another deep copy. I am not sure why the original project creators decided to introduce
that inconsistency. I may change it later.
- `Renderer` contains no game logic and takes in only `const` values to render a snapshot of all logic representations.
- Speed mode is 100% solved without deeply touching classes (handled completely in `main.cpp`).

- Is it possible to go back to one vector of Obstacle class but add overrides and a virtual function to parent class? Attempt this after working project is done.

- How to introduce multiple threads into this game? An idea is have one thread for every moving object (moving obstacles and 2 snakes,
with a future extension being multiple computer controlled snakes). Originally the `Game::Update` method guarantees order of execution of 
Object::Update methods, so there being a change in the environment between reads and writes is impossible. An idea is first update all the obstacles. Computer snakes move around obstacles, so this guarantees the current grid - from there each can compute their new location independently (through multiple threads) and update themselves independently.


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


## Links

Fan of this: https://bumbershootsoft.wordpress.com/2019/04/07/working-with-dlls-on-windows-mac-and-linux/

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines

I read from StackOverflow OpenSSL is good, but I preferred Wei Dai's Crypto++ because I found the instructions clearer to install.

https://github.com/weidai11/cryptopp


Preferred DLL over static library, I do not like bloat:
https://stackoverflow.com/questions/140061/when-to-use-dynamic-vs-static-libraries

https://stackoverflow.com/questions/2321511/what-is-meant-by-resource-acquisition-is-initialization-raii

https://dexp.in/articles/nuklear-intro/
https://cpp.hotexamples.com/examples/-/-/nk_option_label/cpp-nk_option_label-function-examples.html
https://www.geeksforgeeks.org/encrypt-and-decrypt-text-file-using-cpp/
https://linuxhint.com/regular-expression-basics-cpp/
https://cplusplus.com/reference/ctime/ctime/


## Rubric Points for Udacity Graders

| Rubric Criterion | File:Line Number |
|  --------------- | -----------------|
| Understanding of C++ functions and control structures | |
| Read/write data to/from files; process the file data |  |
| Accept and process user input | |
| Data structures and immutable values | |
| One or more classes with appropriate access specifiers for members | |
| Class constructor with member initialization list | | 
| Member functions abstract away implementations and have well documented function names | |
| Overloaded function with multiple parameter signatures | |
| References in function declarations, or using pass-by-reference | |
| Well-implemented destructor | |
| RAII and resource management through scoping | |
| Rule of 5 | |
| Smart pointers | |
| Multi-threading or async tasks | |
| Mutex or lock | |



## Licensing of Starter Project

[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].
