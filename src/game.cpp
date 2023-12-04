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
    // and expecting Render to check for malformed input - increases verbosity of code however
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
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food. - check for obstacles and fake snake too
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  // fake_snake.Update(); 

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // for fixed obstacles, only head can collide
  // for moving obstacles, any part of the snake can collide
  // for fake competitor snake, any part of the snake can collide
  // if (collision) snake.alive = false, return;

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