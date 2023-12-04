#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, bool ai = false)
      : grid_width(grid_width),
        grid_height(grid_height), 
        ai(ai)
        {
          if (!ai) {
            head_x = grid_width/2;
            head_y = grid_height/2;
            direction = Direction::kUp;
          } else {
            head_x = 0;
            head_y = 0;
            direction = Direction::kLeft;
          }
  }

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  bool ai;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif