#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, GameSpeeds speed_mode, GameObstacles obstacle_mode, GameSnakes snake_mode)
    : snake(grid_width, grid_height),
      fake_snake(grid_width, grid_height, true),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      speed_mode(speed_mode),
      obstacle_mode(obstacle_mode),
      snake_mode(snake_mode) {

      PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();

    // call different Render function depending on what set of modes are stored by game
    // decided to make Game handle the modes instead of passing in malformed input
    // and expecting Render to check for malformed input - increases verbosity of code,
    // but also allows further customization per Render method used
    if (snake_mode == GameSnakes::original && obstacle_mode == GameObstacles::noObstacles) {
      renderer.Render(snake, food);
    } else if (snake_mode == GameSnakes::original && (obstacle_mode == GameObstacles::fixedObstacles ||
      obstacle_mode == GameObstacles::mixedObstacles)) {
        renderer.Render(snake, food, obstacles);
    } else if (snake_mode == GameSnakes::computerSnake && obstacle_mode == GameObstacles::noObstacles) {
      renderer.Render(snake, food, obstacles, fake_snake);
    } else if (snake_mode == GameSnakes::computerSnake && (obstacle_mode == GameObstacles::fixedObstacles || 
      obstacle_mode == GameObstacles::mixedObstacles)) {
        renderer.Render(snake, food, obstacles, fake_snake);
    } else {
      //kill game as default case
      running = false;
      return;
    }

    renderer.Render(snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;

  // Create a list of all open spots on the grid, then choose randomly from them
  // next implementation

  // while (true) {
  //   x = random_w(engine);
  //   y = random_h(engine);

  //   food.x = x;
  //   food.y = y;
  //   break;


  //   // Check that the location is not occupied by a snake item before placing
  //   // food. 
  //   if (this->snake_mode == GameSnakes::computerSnake && fake_snake.SnakeCell(x,y)) {
  //       continue;
  //   }
  //   if (snake.SnakeCell(x, y)) continue;


  //   // slight ugly usage of flag
  //   bool hit_obstacle{false};
  //   if (this->obstacle_mode == GameObstacles::fixedObstacles || this->obstacle_mode == GameObstacles::mixedObstacles) {
  //     for (Obstacle obs : obstacles) {
  //       SDL_Point p = obs.leftMostPoint;
  //       if (y != p.y) continue; // continue to next obstacle
  //       for (int i = 0; i < obs.width; ++i) {
  //         if (p.x +i == x) hit_obstacle = true;
  //       }
  //     }
  //   }

  //   if (!hit_obstacle) {
  //     food.x = x;
  //     food.y = y;
  //     return;
  //   }
    
  // }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update(); // collision check happens in UpdateBody
  // fake snake cannot suffer from game terminating collision
  if (snake_mode == GameSnakes::computerSnake) fake_snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (obstacle_mode == GameObstacles::fixedObstacles || obstacle_mode == GameObstacles::mixedObstacles) {
    for (auto obs : obstacles) {
      obs.Update(); // consider if there's an apply function to pass in this too
      // instead of an iterative loop
    }
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }