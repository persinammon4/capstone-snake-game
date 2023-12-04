#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"


class Obstacle {
    public:
        SDL_Point leftMostPoint;
        int width{2}; // mutable from public
        void Update() {return;};

    private:
        int grid_width; 
        int grid_height;
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kRight; //horizontal movement
        float speed{0.1f};    
        int path_size{3};
        int path_progress{0};
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
};

#endif