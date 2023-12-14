#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"


class Obstacle {
    public:
        SDL_Point leftMostPoint;
        int width{2}; // mutable from public
        virtual void Update() {};

    protected:
        int grid_width; 
        int grid_height;
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kUp; //horizontal movement
        int path_size{3};
        int path_progress{0};
        int denominator_slowdown{6};
        void Update() override;
        MovingObstacle(int w, int h) {
            grid_width = w;
            grid_height = h;
        }
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
    public:
        void Update() override;
        FixedObstacle(int w, int h) {
            grid_width = w;
            grid_height = h;
        } 
};

#endif