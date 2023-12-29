# Capstone Snake Game

The Snake game is a game world where a keyboard controlled snake attempts to eat food. Every time it eats food it grows longer by one block, but if it turns in on itself and hits another part of its body it dies. The original implementation set up a basic game loop (Input-Update-Render, all in one time step) including a Snake class, SDL library usage, and rendering of abstract objects. It ran infinitely unless the snake died.

The extended implementation has the following features:
- Multiple Game modes: new AI snake given directions by A* algorithm to go towards the food, with its own collision rules (user snake death if run into it but if user snake hits its tail then a major +1000 score boost - "eating" it). Obstacle modes will place moving or stationary obstacles that will kill either type of snake if the snake runs into it.
- Leaderboard with scores for all previous attempts written to `*.txt` files. I felt very uncomfortable leaving a plaintext file with data public on GitHub, so I implemented encryption with a symmetric key (AES encryption). Encryption is a mandatory feature. Used [Crypto++](https://github.com/weidai11/cryptopp). The static library was recommended over DLL.
- Beginning GUI for user to toggle speed (frames per second) and game modes- different obstacle modes and computer snake mode. Ending GUI with option to add entry to leaderboard or play again. Used [NanoGui-SDL](https://github.com/dalerank/nanogui-sdl) to do so. Originally tried Nuklear (difficult documentation, and apparently has issues even past that for WSL), and Elements (issues with WSL).


The project has in-line comments with reasoning behind implementation decisions. Details further down the README.

Huge additions were made to the original project!


## How to Run

1. Download `build/SnakeGame` and run using `./SnakeGame` from Unix command line in same directory.

Or:

Create own executable by cloning repo.

1. Clone repo.
2. If no build directory, create one `mkdir build`.
3. Compile `cd build && cmake .. && make`.
4. Run `./SnakeGame`


## Testing

This is a freeform game project, so human input and observation is required for testing (not as simple as automatically running a test suite
from command line). Recording scores and gameplay logic are both based on human input. LeaderBoard functions can be tested with unit tests,
but it was faster to visually inspect (this being a personal project, not professional shared amongst a group). It is possible to write
a driver going through all the game scenarios and waiting for a human to play the game once a scenario is triggered.

1. In `main.cpp`, pass in the two game mode parameters that you want to test.
2. To add a new environment (of obstacles and whether to have an AI controlled competitor snake), go to `scenario.h`, write a new `GameEnv[x]` where x is an unused positive integer, and change the order of the method names in the vector to guarantee your function will be used to populate the game. Include an invariant before adding items to Game environment to ensure the new case isn't triggered by a game with the wrong modes.
3. Comment out the GUI in `main.cpp` if necessary.
4. Build and run.


## Notes

- Attempted to replace raw pointers with `weak_ptr`. Ran into an issue where cannot pass in a a weak_ptr of a vector of unique_ptr
through the constructor of controller, leading to constant compilation failures. Ideally, weak_ptr is better than raw pointer 
in that it allows for if statements checking for dealloc of original resource (dangling pointer) and makes the distinction
that this should be an observer pointer more so than a const read-only pointer. During the attempt to implement, it created some points of where
it is possible for there to be nonsensical results (e.g. if `.lock()` returns false). The program is written so that 
the raw pointers are not abused, but I guess weak pointers are good for a larger multi-developer, long-standing project.

- One possible extension of this project is to add more computer controlled snakes. This is an idea of what must be done in order to do that.
    - Create a vector of `fake_snakes` owned by `Game`, and pass a pointer to that in to original snake for it to check against all fake snakes in the vector for its collision rules (in `Snake::UpdateBody`) and to all fake snakes so they can check for collisions amongst themselves.
    - `Controller::AlgorithmInput` needs to be still statically creating `presaved_path` vector for the AI snake to read directions off of, but each snake needs to have its own path vector with no crossover.
    - Fake snakes need to be randomly placed across the board. In the game initialization, all snakes are only 1 block long. Use `Game::returnFreePoint` to randomly place the fake snakes across the board.
    - In beginning GUI, have an option to pick how many fake snakes can be added to the board (integers 0,1,2,3...).
    - In the end leaderboard addition, have a separate leaderboard for each number of AI snakes.
    - Look for a way to use multi-threading to speed up computation of snake movement. However, all movements need to be timed through the game loop, and concurrency would ruin that. The place to consider using multiple threads would be to recalculate A* for example.
    - Extension on extension: have the computer controlled snakes chase the user controlled snake, a negative score deduction for if computer controlled snake eats the food first, a ticking time bomb sudden death type of game, where if the score difference between fake snakes and user controlled snake becomes too large then the game ends and computer snake wins.
    - Potential bug with multiple fake snakes: If they reach the food at approximately the same time, they all may self combust when they run into each other (!!).

- Other possible extensions of this project:
    - Go through and double check if any other places to add `const` and `constexpr`, moves instead of copies based on existing information on [best practice](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines). Is including `static` whenever possible a best practice?
    - Look for memory leaks.
    - The locks, smart pointers, and Rule of 5 already create the basis for this, but ensure that there can be either multiple LeaderBoards sharing access to the single source of truth data files, or one static LeaderBoard where multiple instantiations of this game can share a pointer. Other aspects of gameplay do not need to share data between separate instantiations.
    - Create new scenarios or types of obstacles (e.g. vertically moving obstacles).
    - Add changing colors of obstacles, idea of how to do it written in another bullet point.
    - Have multiple `Game` instances with `LeaderBoards` that share the same encrypted files and copy `shared_ptr`
    resources amongst each other instead of each having their own instance of the file resource. Perhaps implement 
    the ability to write to the decrypted temp files and then encrypted files with every change of data, and to
    re-read the encrypted files and then decrypted temp files. This can be done by letting `LeaderBoard` run in
    a separate thread, using `unique_lock` to lock and unlock the shared data (the encrypted files)
    every time functions are called (so ability to lock and unlock multiple times is necessary), and somehow avoiding
    issues with keys and initialization vectors if one LeaderBoard instance has an issue with those two saved files.
    The end result is that multiple instances of `Game` can be run on different machines with different users,
    but they all share a global, live updated leaderboard with their standing.


- Never ever (ever ever) roll your own encryption (like Geeks4Geeks and other articles/short courses were suggesting):
https://medium.com/@jmayuresh25/create-a-simple-file-encryption-system-in-c-e3726e0f265b
Although, with the character based encryption, if you encrypt the whitespaces by introducing a simple shift in characters by one number, surely that's a way to crack the cipher. How I could know that - I read that Caesar cipher was cracked because the letter "e" in English plaintext is very (statistically) frequent.

- In `LeaderBoard.cpp`, file deletion and creation is driven by the constructor and destructor.

- This project was amenable to being solved quickly, because of the separation of concerns already built in (e.g. Renderer only renders a snapshot of abstract objects, imports go in header files, leaderboard is a file writer independent of other parts of the game, snake collision logic is all in one area, abstract object movement is handled inside of the object itself through the Update method, controller just needs directions fed into it allowing for A* to only focus on returning directions instead of handling updating the points themselves, GUI and LeaderBoard can be the last to be implemented).

- `Renderer` and `Controller` don't own any of the class representations e.g. Snake, Obstacle. They just take in references or 
copies and either draw out a renderering from a `const` object or mutate values within the object. The food is 
implemented as an `SDL_Point` and not a class, which is perfect, but for some reason it's being passed in as a reference to 
the `Renderer` instead of another copy. I am not sure why the original project creators decided to introduce
that inconsistency. I may change it later.

- Figured out how to make flashing colored items! Have two `SDL_SetRenderDrawColor`s next to each other and it will effectively be a strobe light!

- Speed mode is 100% solved without deeply touching classes (handled completely in `main.cpp`).

- "The ownership of an object can only be shared with another `shared_ptr` by copy constructing or copy assigning its value to another `shared_ptr`. Constructing a new `shared_ptr` using the raw underlying pointer owned by another `shared_ptr` leads to undefined behavior."


## Notes about A*

A* moves the computer controlled snake by having a method be given a snake and change its direction based on a one time computed vector of directions. (Technically, this method could be fed the user controlled snake to overwrite the user inputted commands for that snake in the Input-Update-Render loop). To read the directions, static variables are declared in `Controller::AlgorithmInput`, a vector is assigned, and direction is read and updated by reading without writing to vector and re-computing a new path if hit the end of the vector. Originally, the attempt was to have a vector, use `vector.erase(vector.begin())` to pop off element 0, and then re-compute a vector if `vector.size() == 0`. This led to no changes in direction. Also, the frame rate means movement is fractional not just integer based like the SDL_Points the nodes are based off (e.g. if snake speed is `0.1f`, that means it takes ten steps at the same direction before the snake reaches the next point/node and can switch to the next direction).

If there are going to be multiple snakes the `static` count and the `% [int]` can be reused, but the vectors need to be separated.
There needs to be a `dir_index` for each vector, because each vector can have different sizes so doing something like messing with the count variable will make it messier than necessary (reasoning behind why in the current implementation there's both a count and a dir_index instead of only count).

<img src="snake_game.gif"/>

## Dependencies for Running Locally

I developed on WSL2.

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

* For the Crypto++ Library, installed using `sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils`. 
This would have been much more painful if not on Linux by the way. Without a Linux machine or learning to install on
another computer, only the executable is valuable. During build, a new submodule/Git repo will be created `build/cryptopp-cmake/cryptopp`.

* [NanoGui for SDL](https://github.com/dalerank/nanogui-sdl) was constructed well so just needed to be cloned in and linked through header files.

## Links

https://web.archive.org/web/20150315102342/http://www.cryptopp.com/fom-serve/cache/79.html
https://www.cryptopp.com/wiki/Advanced_Encryption_Standard

https://crypto.stackexchange.com/questions/1129/can-cbc-ciphertext-be-decrypted-if-the-key-is-known-but-the-iv-not
https://stackoverflow.com/questions/75650231/cmake-cant-find-cryptopp-with-find-package

https://www.udacity.com/blog/2021/05/how-to-read-from-a-file-in-cpp.html

https://stackoverflow.com/questions/43114174/convert-a-string-to-std-filesystem-path

Unbelievably, this helped upgrade from `cmake 3.22` to `cmake 3.28`: https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line. My version of Ubuntu through WSL was the Jammy Jellyfish one. 

Fan of this: https://bumbershootsoft.wordpress.com/2019/04/07/working-with-dlls-on-windows-mac-and-linux/

Whoever designed this Udacity Nanodegree appears to be current on the Cpp Core Guidelines:
https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
Was able to check off that used the Nanodegree ideas effectively, but (obviously) unable to go through something mammoth like this.

Crypto library-
https://github.com/weidai11/cryptopp
Originally attempted to use DLL over static, but later found guideline to use static.


https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful
https://en.cppreference.com/w/cpp/memory/weak_ptr/weak_ptr

https://stackoverflow.com/questions/2321511/what-is-meant-by-resource-acquisition-is-initialization-raii

https://dexp.in/articles/nuklear-intro/
https://cpp.hotexamples.com/examples/-/-/nk_option_label/cpp-nk_option_label-function-examples.html
https://www.geeksforgeeks.org/encrypt-and-decrypt-text-file-using-cpp/
https://linuxhint.com/regular-expression-basics-cpp/
https://cplusplus.com/reference/ctime/ctime/
https://stackoverflow.com/questions/654713/o-files-vs-a-files
https://cplusplus.com/reference/


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
