#ifndef OBSTACLE_H
#define OBSTACLE_H

// relevant includes
// flesh out every class related to obstacle
#include "SDL.h"


class Obstacle {
    public:
        std::vector<SDL_Point> platform;

    private:
        int grid_width;
        int grid_height;
        int width{1};
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kUp;
        float speed{0.1f};    
        int path_size{3};
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
};

#endif