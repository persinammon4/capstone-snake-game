#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"
#include <vector>
#include <memory>

enum class GameSpeeds { slow, medium, fast};
enum class GameObstacles { noObstacles, fixedObstacles, mixedObstacles };
enum class GameSnakes { original, computerSnake };

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, GameSpeeds speed_mode, GameObstacles obstacle_mode, GameSnakes snake_mode);
  void Run(Renderer &renderer,
           std::size_t target_frame_duration); 
  int GetScore() const;
  int GetSize() const;

  void addFixedObstacle(int width);
  void addMovingObstacle(int width, int path_size);

  // user given modes, unable to set after init
  // but publicly readable
  GameSpeeds const speed_mode;
  GameObstacles const obstacle_mode;
  GameSnakes const snake_mode;

 private:
 // these two objects are not pointers of any sort. they are declared on the stack in the game constructor and implicitly automatically destroyed in the game destructor
  Snake snake; 
  SDL_Point food;

  // note that leaderboard is not owned by Game, but Game outputs are written to leaderboard in main.cpp
  // unique pointers allow for automatic RAII by Game object 
  std::vector<std::unique_ptr<Obstacle>> obstacles;
  // always declare this - is explicitly init to either nullptr or a right type of snake in Game constructor
  std::unique_ptr<Snake> fake_snake;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  std::size_t grid_height;
  std::size_t grid_width;

  int score{0};

  SDL_Point returnFreePoint(int size);

  void PlaceFood();
  void Update();
  std::vector<Obstacle> getReadOnlyObstacles();

  Controller controller;
};

#endif