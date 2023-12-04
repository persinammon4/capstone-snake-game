#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
  void AlgorithmInput(bool &running, Snake &fake_snake) const; // handle A* search to decide next direction

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  // helper functions for AlgorithmInput
  int Heuristic(); // calculate Manhattan distance between two points
  // check valid cell
  // compare f values
};

#endif