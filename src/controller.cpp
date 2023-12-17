#include "controller.h"
#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return; // can lead to same direction as before used if given an invalid direction
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  // don't want to introduce try catch into the caller function so have a logging print statement and early break
  if (snake.ai) {
    std::cout << "Programmer error: Used HandleInput for an AI snake.";
    return; // quick sanity check for programmer error
  }
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

void print_vec(std::vector<Snake::Direction> v) {
  std::cout << "printing out a vec: ";
  for (auto e : v) {
    switch(e) {
      case Snake::Direction::kDown:
        std::cout << " down";
        break;
      case Snake::Direction::kUp:
        std::cout << " up";
        break;
      case Snake::Direction::kRight:
        std::cout << " right";
        break;
      case Snake::Direction::kLeft:
        std::cout << " left";
        break;
    }
  }
  std::cout << "\n";
}

void print_dir(Snake::Direction d) {
  std::cout << "print out single dir: ";
  switch(d) {
  case Snake::Direction::kDown:
    std::cout << " down";
    break;
  case Snake::Direction::kUp:
    std::cout << " up";
    break;
  case Snake::Direction::kRight:
    std::cout << " right";
    break;
  case Snake::Direction::kLeft:
    std::cout << " left";
    break;
  }
  std::cout << "\n";
}

void print_open_list(std::vector<Controller::Node> open_list) {
  std::cout << "printing out an open list completely ordered: ";
  for (Controller::Node o : open_list) {
    std::cout << "{ " << o.x << ", " << o.y << " } with f-val " << o.g+o.h << ";";
  }
  std::cout << "\n";
}

void Controller::AlgorithmInput(bool &running, Snake &fake_snake) {
  static int count = 0;
  static int dir_index = 0;
  static std::vector<Snake::Direction> presaved_path{Snake::Direction::kRight};

  // don't want to introduce try catch into the caller function so have a logging print statement and early break
  if (!fake_snake.ai) {
    std::cout << "Programmer error: Used AlgorithmInput for a user snake.";
    return; // quick sanity check for programmer error
  } 


  // if food location changed
  // if there is no path already saved
  // later, if an obstacle changed (to create computer snakes that do not collide
  if (presaved_path.size() == 0 || food_ptr-> x != food_snapshot.x || food_ptr->y != food_snapshot.y) {
      food_snapshot.x = food_ptr->x; // will update, even if it updates to same thing e..g the original food and food snapshot are still the same
      food_snapshot.y = food_ptr->y; // makes it easier to read
      // print_vec(presaved_path);
      presaved_path = Search(fake_snake); // A* is not constructing a valid path?? -
      // print_vec(presaved_path);
  }

  // Every 10 moves change direction
  count++;
  if (count % 10 == 0) {
    if (dir_index < presaved_path.size()) {
      direction = presaved_path[dir_index];
      dir_index++;
    } else {
      dir_index = 0;
      presaved_path = Search(fake_snake);
    }
  }

  // attempts to update computer controlled snake to determined next direction, if 
  // it's invalid somehow, will skip this cycle but keep checking next cycle if change in grid
  switch(direction) {
    case Snake::Direction::kUp:
      ChangeDirection(fake_snake, direction, Snake::Direction::kDown);
      break;
    case Snake::Direction::kDown:
      ChangeDirection(fake_snake, direction, Snake::Direction::kUp);
      break;
    case Snake::Direction::kLeft:
      ChangeDirection(fake_snake, direction, Snake::Direction::kRight);
      break;
    case Snake::Direction::kRight:
      ChangeDirection(fake_snake, direction, Snake::Direction::kLeft);
      break;
    default:
      // leads to a standstill, btw case never triggered
      break;
  }
}

// Helper functions for AlgorithmInput

std::vector<Snake::Direction> Controller::Search(Snake &fake_snake) {
  
  // initialization of A*
  std::vector<Controller::Node> open_list; 
  SDL_Point p; //populate with current location
  p.x = fake_snake.head_x;
  p.y = fake_snake.head_y;
  int g = 0; // g-value when started is 0, this is path length from starting point


  // adds snapshot of obstacles, so computer controlled snake knows to avoid obstacles
  // using 2 for obstacles, so can do a run through and remove them during every update
  std::vector<int> vec(grid_width, 0);
  temp_grid_Astar.clear();
  for (int i = 0; i < grid_height; ++i) temp_grid_Astar.push_back(vec); // push_back makes copy
  for (int i = 0; i < obstacles->size(); ++i) { // no for range loop to avoid unique_ptr problems during implicit copy
    int y = (*obstacles)[i]->leftMostPoint.y;
    int temp_x = (*obstacles)[i]->leftMostPoint.x;
    for (int k = 0; k < (*obstacles)[i]->width; ++k) {
      temp_grid_Astar[y][temp_x+k] = 2;
    } 
  }

  std::vector<Snake::Direction> prev_dirs;
  AddToOpen(p.x, p.y, g, Heuristic(p, p), prev_dirs, Snake::Direction::kRight, open_list); // the last var is pass-by-reference to mutate original

  while (open_list.size() > 0) { 
    // std::cout << "1\n";
    // print_open_list(open_list);
    CellSort(open_list);
    // std::cout << "2\n";
    // print_open_list(open_list);
    Controller::Node closest_node = open_list[0];
    open_list.erase(open_list.begin());
    // std::cout << "3\n";
    // print_open_list(open_list);
    temp_grid_Astar[closest_node.y][closest_node.x] = 1; //visited this
    if (closest_node.x == food_snapshot.x && closest_node.y == food_snapshot.y) {
      // std::cout << " found the food \n";
      return closest_node.prev_directions;
    }
    ExpandNeighbors(closest_node, open_list); // do something here
  }

  // so it's forced to return something and fulfill the signature
  std::vector<Snake::Direction> optimal_path; // empty path
  return optimal_path;
}



void Controller::CellSort(std::vector<Controller::Node> &open_list) {
  // compares f-values and returns true if f-val of first is greater than that of second (f = g prev path length + h heuristic defined in Heuristic func)
  std::sort(open_list.begin(), open_list.end(), [](Controller::Node node1, Controller::Node node2){ return (node1.g + node1.h) < (node2.g + node2.h); });
}

void Controller::ExpandNeighbors(Controller::Node currentnode, std::vector<Controller::Node> &open_list) {
  SDL_Point p;

  // is this method working?????

  // if matrix[y][x] == 0, then unchecked and free, if == 2 then obstacle, if == 1 then already visited during this run of A*

  // handles wrap around here
  // assumes previous location is valid
  //up
  p.x = currentnode.x;
  p.y = currentnode.y-1; 
  if (p.y < 0) p.y = grid_height-1; //wraparound
  if (temp_grid_Astar[p.y][p.x] == 0) AddToOpen(p.x, p.y, currentnode.g+1, Heuristic(p, food_snapshot), currentnode.prev_directions, Snake::Direction::kUp, open_list);
  // down
  p.y = currentnode.y+1; 
  if (p.y >= grid_height) p.y = 0; // wraparound 
  if (temp_grid_Astar[p.y][p.x] == 0) AddToOpen(p.x, p.y, currentnode.g+1, Heuristic(p, food_snapshot), currentnode.prev_directions, Snake::Direction::kDown, open_list);
  // left
  p.x = currentnode.x-1; 
  p.y = currentnode.y;
  if (p.x < 0) p.x = grid_width-1; // wraparound
  if (temp_grid_Astar[p.y][p.x] == 0) AddToOpen(p.x, p.y, currentnode.g+1, Heuristic(p, food_snapshot), currentnode.prev_directions, Snake::Direction::kLeft, open_list);
  // right
  p.x = currentnode.x+1; 
  if (p.x >= grid_width) p.x = 0; //wraparound 
  if (temp_grid_Astar[p.y][p.x] == 0) AddToOpen(p.x, p.y, currentnode.g+1, Heuristic(p, food_snapshot), currentnode.prev_directions, Snake::Direction::kRight, open_list);

}

int Controller::Heuristic(SDL_Point p1, SDL_Point p2) { // could technically declare as const, but because not a member data read using const is semantically nonsensical 
    return abs(p2.x - p1.x) + abs(p2.y - p1.y); // Manhattan ("taxi cab") distance; easiest example of different heuristics/measures for layman is to think of Euclidean distance taught in school
}

void Controller::AddToOpen(int x, int y, int g, int h, std::vector<Snake::Direction> prev_dirs, Snake::Direction next_dir, std::vector<Controller::Node> &open_list) {
  open_list.emplace_back(x,y,g,h, prev_dirs);
  // prev directions should have complete path
  open_list.back().prev_directions.push_back(next_dir);
  temp_grid_Astar[y][x] = 1;
}
