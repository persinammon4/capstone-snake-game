#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "obstacle.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, bool ai = false)
      : grid_width(grid_width),
        grid_height(grid_height), 
        ai(ai)
        {
          if (!ai) {
            // if user controlled snake, initialize in middle of grid
            head_x = grid_width/2;
            head_y = grid_height/2;
            direction = Direction::kUp;
          } else {
            // if it's computer controlled snake,
            // initialize from the top left
            head_x = 0;
            head_y = 0;
            direction = Direction::kRight;
            //speed = 1.0f;
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
  bool flag_slain{false}; // edge case where ai snake can be killed
  std::vector<SDL_Point> body;

  // own raw pointers to other snake and obstacles both created by Game
  // no explicit const against modification of resource and it's a direct reference
  // so take care not to modify these resources and ruin gameplay with magic logic
  Snake * fake_snake; // nullptr if it's the fake snake, so if manipulate nullptr will cause seg fault
  // fake snake has no pointer to real_snake, as it's unnecessary for it to know
  std::vector<std::unique_ptr<Obstacle> >* obstacles;

 private:

  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;

};

#endif