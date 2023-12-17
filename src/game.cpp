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
      snake_mode(snake_mode),
      grid_width(grid_width),
      grid_height(grid_height),
      controller(&food, grid_width, grid_height, &obstacles) {

      snake.obstacles = &obstacles; // this is a pointer within snake to game level unique_ptr obstacles, original truth
      if (snake_mode == GameSnakes::computerSnake) {
        auto s = new Snake(grid_width, grid_height, true);
        fake_snake = std::make_unique<Snake>(*s);
        snake.fake_snake = fake_snake.get();
        fake_snake->obstacles = &obstacles; // this is a pointer within fake_snake to game level unique_ptr obstacles, original truth
      } else if (snake_mode == GameSnakes::original) {
        fake_snake = nullptr;
      }
      // obstacles are initialized outside of constructor, so updated at time of obstacle change
      // no obstacles made or manipulated here

      PlaceFood();
}


void Game::Run(Renderer &renderer,
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

    if (snake_mode == GameSnakes::computerSnake) {
      controller.AlgorithmInput(running, *fake_snake);
    }
    
    Update();

    // call different Render function depending on what set of modes are stored by game
    // decided to make Game handle the modes instead of passing in malformed input for one Render function
    // and expecting Render to check for the malformed input
    // much cleaner and able to make further customization for each game case e.g. different colors during Render
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
  // food.x = 0;
  // food.y = 0;
  food = returnFreePoint(1); // originally this function was for obstacle placement, but found that can reuse for food (!!)
}

void Game::Update() {
  if (!snake.alive) return; // only if user controlled snake suffers a collision is game terminated


  snake.Update(); // user snake collision check happens in UpdateBody

  if (snake_mode == GameSnakes::computerSnake) 
  {
    if (fake_snake->flag_slain) {
      score+=1000; // computer controlled snake death leads to score bump and no game termination
      fake_snake->flag_slain = false;
    }
    if (fake_snake->alive) {
      fake_snake->Update(); // computer controlled snake collision check happens in UpdateBody
    } 
    // fake snake grows if it eats food; static_cast is reused from earlier code and allows compiler to create a compiler time error for mismatched types instead of runtime error
    int new_x_fake = static_cast<int>(fake_snake->head_x);
    int new_y_fake = static_cast<int>(fake_snake->head_y);

    if (food.x == new_x_fake && food.y == new_y_fake) {
      PlaceFood();
      fake_snake->GrowBody();
      // fake_snake->speed += 0.02; fake_snake does not accelerate after eating each food unlike user controlled snake
      // fake snake needs to restart A* somehow
    }
  }

  // static_cast is used because reused from earlier project given code, and allows for compiler to create compile time error for mismatched types instead of runtime error
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (obstacle_mode == GameObstacles::fixedObstacles || obstacle_mode == GameObstacles::mixedObstacles) {
    for (int i = 0; i < obstacles.size(); ++i) obstacles[i]->Update(); // some Obstacles move on Update, others remain in same position (stub Update method)
    // the following for_each did not work because obstacles is unique pointers not raw, the lambda is pass by copy
    // and the workaround to use a move to get the parameter in was time consuming to debug 
    //std::for_each(obstacles.begin(), obstacles.end(), [](auto o){ o.Update();});
  }


  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++; // may be broken??
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

}

void Game::addFixedObstacle(int width) {
  auto item = new FixedObstacle((int) grid_width, (int) grid_height); // heap managed obstacle guarantees persistence over different scope, and semantically makes sense
  item->width = width;
  item->leftMostPoint = returnFreePoint(width);
  obstacles.emplace_back(item); //emplace creates a unique_ptr managing the Obstacle, no need for a corresponding delete
}

void Game::addMovingObstacle(int width, int path_size = 3) {
  auto item = new MovingObstacle((int) grid_width, (int) grid_height); // heap managed obstacle guarantees persistence over different scope, and semantically makes sense
  item->width = width;
  item->leftMostPoint = returnFreePoint(width);
  item->path_size = path_size;
  obstacles.emplace_back(item); //emplace creates a unique_ptr managing the Obstacle, no need for a corresponding delete
}

std::vector<Obstacle> Game::getReadOnlyObstacles() {
  std::vector<Obstacle> read_only_obstacles;
  for (size_t i = 0; i < obstacles.size(); ++i) {
    Obstacle o = *obstacles[i];
    read_only_obstacles.push_back(o); // this creates a copy, so modification doesn't affect original
    // to save a copy, it'd be cool to get it to work for const Obstacle pointers where
    // the Obstacle is read-only (and I guess the pointer can be unmodifiable too, but it's not the relevant part)
    // originally tried to many errors 
  }

  // I originally tried to get an optimized one liner instead of an imperative for loop
  // e.g. the transform method
  // unfortunately, this is difficult to make work for unique_ptr instead of raw pointer which needs to be moved into a lambda func
  // even the workaround (auto o=move(x)) as lambda parameter was difficult to figure what would work
  //std::transform(obstacles.cbegin(), obstacles.cend(), read_only_obstacles.begin(), [](auto &&o) { return o.get();});
  return read_only_obstacles;
}

// Finds a left most point to place an obstacle (moving or fixed) or food
// which is int size. Am OK with things like triple for loops as this is a setup step,
// possibly the scenario method calling this can become some kind of constexpr compile time method
// this is also used for food placement, but does not seem to slow down the placement to an unplayable degree
SDL_Point Game::returnFreePoint(int size) { // works for all horizontal obstacles, not vertically stacked obstacles which are currently unimplemented anyway

  // P.S. this will never happen because a good game play is priority #1 and the snakes need space,
  // but this function does not have a way to handle there
  // being no good points or there being good points but so limited that obstacles and food clash with each other
  // (another version of no good points, but harder to debug e.g. food vacillating between two locations, or
  // being sent in the way of an obstacle path)

  // fyi, because push_back creates a copy
  // it's ok to reuse zero_vec and throwaway
  std::vector<int> zero_vec(grid_width-1,0);
  std::vector<std::vector<int>> occupied_points_matrix;
  for (int i = 0; i < grid_width; ++i) occupied_points_matrix.push_back(zero_vec);

  SDL_Point throwaway;

  // now proceed to use throwaway to add in all occupied points in a data structure
  // will use this to populate a 0-1 grid for O(1) access of current layout
  throwaway.x = (int) snake.head_x;
  throwaway.y = (int) snake.head_y;
  occupied_points_matrix[throwaway.y][throwaway.x] = 1;

  // using this to create new placement for food too, guarantees food doesn't repeat placements
  occupied_points_matrix[food.y][food.x] = 1;

  for (SDL_Point p : snake.body) {
     occupied_points_matrix[p.y][p.x] = 1;
  }

  if (snake_mode == GameSnakes::computerSnake) {
    throwaway.x = fake_snake->head_x;
    throwaway.y = fake_snake->head_y;
    occupied_points_matrix[throwaway.y][throwaway.x] = 1;

    for (SDL_Point p : fake_snake->body) {
      occupied_points_matrix[p.y][p.x] = 1;
    }
  }
  
  auto obs_vec = getReadOnlyObstacles();
  for (auto obs : obs_vec) {
    throwaway.y = obs.leftMostPoint.y;
    // add in extra spots for the entire path of the obstacle, calculated now instead of stored by obstacle
    for (int i = 0; i < obs.width; ++i) {
      throwaway.x = obs.leftMostPoint.x + i;
      occupied_points_matrix[throwaway.y][throwaway.x] = 1;
    }
  }

  std::vector<SDL_Point> available_points;
  for (int j = 0; j < grid_height; ++j) {
    for (int i = 0; i < grid_width; ++i) {
      for (int k = 0; k < size; ++k) {
        if (occupied_points_matrix[j][i+k] == 1) break;
        if (occupied_points_matrix[j][i+k] == 0 && k == size-1) {
          throwaway.x = i;
          throwaway.y = j;
          available_points.push_back(throwaway);
        }
      }
    }
  }

  // access a random point that works and return it
  int randomIndex = rand() % available_points.size();
  return available_points[randomIndex];
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }