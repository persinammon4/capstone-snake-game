#include "game.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <map>
#include <stdlib.h>    
#include <typeinfo>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, GameSpeeds speed_mode, GameObstacles obstacle_mode, GameSnakes snake_mode)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      speed_mode(speed_mode),
      obstacle_mode(obstacle_mode),
      snake_mode(snake_mode) {

      snake.fixed_obstacles = &fixed_obstacles;
      snake.moving_obstacles = &moving_obstacles;
      if (snake_mode == GameSnakes::computerSnake) {
        auto s = new Snake(grid_width, grid_height, true);
        fake_snake = std::make_unique<Snake>(*s);
        snake.fake_snake = s;
        fake_snake->fixed_obstacles = &fixed_obstacles;
        fake_snake->moving_obstacles = &moving_obstacles;
      }
      // obstacles are initialized outside of constructor, so updated at time of obstacle change

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

  // auto vec = getReadOnlyObstacles();
  // for (size_t i = 0; i < obstacles.size(); ++i) {
  //   std::cout << obstacles[i]->leftMostPoint.x << " " << obstacles[i]->leftMostPoint.y << std::endl;
  // }
  // std::cout << obstacles.size() << std::endl;

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
        renderer.Render(snake, food, getReadOnlyObstacles());
    } else if (snake_mode == GameSnakes::computerSnake && obstacle_mode == GameObstacles::noObstacles) {
        renderer.Render(snake, food, getReadOnlyObstacles(), *fake_snake);
    } else if (snake_mode == GameSnakes::computerSnake && (obstacle_mode == GameObstacles::fixedObstacles || 
      obstacle_mode == GameObstacles::mixedObstacles)) {
        renderer.Render(snake, food, getReadOnlyObstacles(), *fake_snake);
    } else {
      //kill game as default case
      running = false;
      return;
    }

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
  //food = returnFreePoint(1);
}

void Game::Update() {
  if (!snake.alive) return;


  snake.Update(); // collision check happens in UpdateBody
  // fake snake cannot suffer from game terminating collision

  if (snake_mode == GameSnakes::computerSnake) 
  {
    if (fake_snake->alive) {
      fake_snake->Update();
      if (!fake_snake->alive) score += 100;
    } 
  }


  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (obstacle_mode == GameObstacles::fixedObstacles) {
    for (int i = 0; i < fixed_obstacles.size(); ++i) {
      fixed_obstacles[i]->Update();
    }
    //std::for_each(obstacles.begin(), obstacles.end(), [](auto o){ o.Update();});
  }

  if (obstacle_mode == GameObstacles::mixedObstacles) {
    for (int i = 0; i < fixed_obstacles.size(); ++i) {
      fixed_obstacles[i]->Update();
    }
    for (int i = 0; i < moving_obstacles.size(); ++i) {
      moving_obstacles[i]->Update();
    }
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++; // may be broken??
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // fake snake grows if it eats food
  int new_x_fake = static_cast<int>(fake_snake->head_x);
  int new_y_fake = static_cast<int>(fake_snake->head_y);

  if (food.x == new_x_fake && food.y == new_y_fake) {
    PlaceFood();
    fake_snake->GrowBody();
    // fake_snake->speed += 0.02;
  }
}

void Game::addFixedObstacle(int width) {
  auto item = new FixedObstacle((int) grid_width, (int) grid_height);
  item->width = width;
  SDL_Point p;
  p.x = 5;
  p.y = 5;
  item->leftMostPoint = p;
  //item->leftMostPoint = returnFreePoint(width);
  fixed_obstacles.emplace_back(item); // constructs new unique pointer with argument item
}

void Game::addMovingObstacle(int width, int path_size = 3) {
  auto item = new MovingObstacle((int) grid_width, (int) grid_height);
  item->width = width;
  SDL_Point p;
  p.x = 10;
  p.y = 10;
  item->leftMostPoint = p;
  //item->leftMostPoint = returnFreePoint(width);
  item->path_size = path_size;
  moving_obstacles.emplace_back(item);
}

std::vector<Obstacle> Game::getReadOnlyObstacles() {
  // implement logic here
  std::vector<Obstacle> read_only_obstacles;
  for (size_t i = 0; i < fixed_obstacles.size(); ++i) {
    Obstacle o = *fixed_obstacles[i];
    read_only_obstacles.push_back(o);
    //read_only_obstacles.push_back((Obstacle) (*fixed_obstacles[i])); //pushes a copy
  }
  for (size_t i = 0; i < moving_obstacles.size(); ++i) {
    Obstacle o = *moving_obstacles[i];
    read_only_obstacles.push_back(o);
    //read_only_obstacles.push_back((Obstacle) (*moving_obstacles[i])); //pushes a copy
  }

  // I tried to use transform method, but several attempts to operatoe on unique pointers led to hard to debug errors
  //std::transform(obstacles.cbegin(), obstacles.cend(), read_only_obstacles.begin(), [](auto &&o) { return o.get();});
  return read_only_obstacles;
}

// Finds a left most point to place an obstacle (moving or fixed)
// which is int size. Am OK with inefficiency as this is a setup step,
// possibly the scenario method calling this can become some kind of constexpr compile time method
SDL_Point Game::returnFreePoint(int size) { // works for all horizontal obstacles

  std::vector<SDL_Point> occupied_points_vector;

  // fyi, because push_back creates a copy, it's ok to 
  // reuse throwaway across the function
  SDL_Point throwaway;
  // throwaway.x = 1;
  // throwaway.y = 5;
  return throwaway;

  // throwaway.x = (int) snake.head_x;
  // throwaway.y = (int) snake.head_y;
  // occupied_points_vector.push_back(throwaway);

  // occupied_points_vector.push_back(food);

  // for (SDL_Point p : snake.body) {
  //    occupied_points_vector.push_back(p);
  // }

  // if (snake_mode == GameSnakes::computerSnake) {
  //   throwaway.x = fake_snake->head_x;
  //   throwaway.y = fake_snake->head_y;
  //   occupied_points_vector.push_back(throwaway);

  //   for (SDL_Point p : fake_snake->body) {
  //     occupied_points_vector.push_back(p);
  //   }
  // }
  
  // auto obs_vec = getReadOnlyObstacles();
  // for (auto obs : obs_vec) {
  //   throwaway.y = obs.leftMostPoint.y;
  //   // add in extra spots for the entire path of the obstacle
  //   for (int i =  0; i < obs.width; ++i) {
  //     throwaway.x = obs.leftMostPoint.x + i;
  //     occupied_points_vector.push_back(throwaway);
  //   }
  // }

  // // create a list of occupied points as a 0-1 matrix
  // std::vector<std::vector<int>> occupied_points_matrix;
  // for (size_t i = 0; i < grid_height; ++i) {
  //   std::vector<int> temp_vect(grid_width, 0);
  //   occupied_points_matrix.push_back(temp_vect);
  // }

  // for (SDL_Point p : occupied_points_vector) {
  //   occupied_points_matrix[p.y][p.x] = 1;
  // }

  // std::vector<SDL_Point> potential_points;
  // for (size_t i = 0; i < grid_height; ++i) {
  //   for (size_t j = 0; j < grid_width; ++j) {
  //     for (int k = 0; k < size; ++i) {
  //       if (occupied_points_matrix[i][j] == 1) {
  //         break;
  //       } else if (k == size-1) {
  //         throwaway.x = j;
  //         throwaway.y = i;
  //         potential_points.push_back(throwaway);
  //       }
  //     }
  //   }
  // }

  // // access a random point that works and return it
  // int randomIndex = rand() % potential_points.size();
  // return potential_points[randomIndex];
  // // not returning ?? 
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }