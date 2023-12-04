#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "SDL.h"

#include<vector>

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
  void AlgorithmInput(bool &running, Snake &fake_snake) const; // contains A* search to decide next direction

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  // helper functions for AlgorithmInput
  // what's f versus g value
  int CalculateHValue(); // calculate Manhattan distance between two points
  bool CheckInGrid(SDL_Point const potentialCoord);
  void AddNeighbors(std::vector<SDL_Point> &open_list, SDL_Point const &potentialCoord); //open_list is not global, so passed in and mutated
  bool Compare(SDL_Point const &a, SDL_Point const &b);
  SDL_Point NextNode(std::vector<SDL_Point> &open_list); //sorts list and returns the node with the best h value
};

#endif