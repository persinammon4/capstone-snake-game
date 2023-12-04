#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"
#include <vector>

enum class GameSpeeds { slow, medium, fast};
enum class GameObstacles { noObstacles, fixedObstacles, mixedObstacles };
enum class GameSnakes { original, computerSnake };

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, GameSpeeds speed_mode, GameObstacles obstacle_mode, GameSnakes snake_mode);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration); 
  int GetScore() const;
  int GetSize() const;

  // user given modes, unable to set after init
  // but publicly readable
  GameSpeeds const speed_mode;
  GameObstacles const obstacle_mode;
  GameSnakes const snake_mode;

 private:
  Snake snake;
  SDL_Point food;

  // always declare these, even if game mode ends up not using them
  // note that leaderboard is not owned by Game, but Game outputs are written to leaderboard
  // should these be unique pointers?
  std::vector<Obstacle> obstacles;
  Snake fake_snake; 

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif