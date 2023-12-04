#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"


class Obstacle {
    public:
        SDL_Point leftMostPoint;
        int width{2}; // mutable from public

    private:
        int grid_width; 
        int grid_height;
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kRight;
        float speed{0.1f};    
        int path_size{3};

        // MovingObstacle(); - default constructor with initializer list of const vars
        // declare any other methods that go into this class
        // how to update position
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
};

#endif