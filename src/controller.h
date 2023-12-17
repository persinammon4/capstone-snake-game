#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "SDL.h"

#include<vector>

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

  // contains A* search to decide next direction for computer controlled snake, outputs directions, equivalent of HandleInput 
  void AlgorithmInput(bool &running, Snake &fake_snake);

  struct Node {
    int x;
    int y;
    int g; // path length in terms of number of nodes/SDL points traversed so far; g + h = f used to order search list
    int h; // absolute distance between start node and current node (currently impl as Manhattan Distance in Heuristic func); g + h = f used to order search list
    std::vector<Snake::Direction> prev_directions; // length g vector of directions to current node from start node
    Node (int x, int y, int g, int h, std::vector<Snake::Direction> prev_directions) : x(x), y(y), g(g), h(h), prev_directions(prev_directions) {}
  };

  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  // helper functions for AlgorithmInput
  std::vector<Snake::Direction> Search(Snake &fake_snake);
  void CellSort(std::vector<Controller::Node> &open_list);
  bool Compare(Controller::Node node1, Controller::Node node2);
  int Heuristic(SDL_Point p1, SDL_Point p2); 
  void AddToOpen(int x, int y, int g, int h, std::vector<Snake::Direction> prev_dirs, Snake::Direction next_dir, std::vector<Controller::Node> &open_list);
  void ExpandNeighbors(Controller::Node currentnode, std::vector<Controller::Node> &open_list);
  void AddToOpen(int x, int y, int g, int h, std::vector<Controller::Node> &open_list);


  // do I need to implement Rule of 5???
  Controller(SDL_Point* food_ptr, int grid_width, int grid_height, std::vector<std::unique_ptr<Obstacle>>* obstacles) : food_ptr(food_ptr), grid_width(grid_width), grid_height(grid_height), obstacles(obstacles) {
    food_snapshot = *food_ptr;

    std::vector<int> vec(grid_width, 0);
    for (int i = 0; i < grid_height; ++i) temp_grid_Astar.push_back(vec); // push_back makes copy

  }

private:
  SDL_Point food_snapshot;
  SDL_Point* food_ptr;
  std::vector<std::vector<int>> temp_grid_Astar;
  int grid_width;
  int grid_height;
  std::vector<std::unique_ptr<Obstacle>>* obstacles;
  Snake::Direction direction;

};

#endif