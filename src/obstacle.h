#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"


class Obstacle {
    public:
        SDL_Point leftMostPoint;
        int width{2}; // mutable from public
        //void Update() {return;};

    // protected:
    //     int grid_width; 
    //     int grid_height;
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kRight; //horizontal movement
        int path_size{3};
        int path_progress{0};
        void Update();
        MovingObstacle(int grid_width, int grid_height) : grid_width(grid_width),
        grid_height(grid_height) {}
    private:
        int grid_width;
        int grid_height;
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
    public:
        void Update();
        FixedObstacle(int grid_width, int grid_height) : grid_width(grid_width),
        grid_height(grid_height) {}
    private:
        int grid_width;
        int grid_height;
};

#endif