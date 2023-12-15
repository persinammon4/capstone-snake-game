#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"


class Obstacle {
    public:
        SDL_Point leftMostPoint;
        int width{2}; // mutable from public
        virtual void Update() {};

    protected: // can be used by derived classes but not public, private would not allow derived classes to use directly
        int grid_width; 
        int grid_height;
};

class MovingObstacle : public Obstacle {
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Direction direction = Direction::kRight; //horizontal movement
        int path_size{3};
        int path_progress{0};
        int denominator_slowdown{6};
        void Update() override; // override keyword was necessary to get this to work well with vec of parent class
        MovingObstacle(int w, int h) { // don't attempt an initializer list, caused a huge error from a huge int being stored in g_w and g_h previously
            grid_width = w;
            grid_height = h;
        }
};

class FixedObstacle : public Obstacle {
    // no additional information necessary
    public:
        void Update() override;  // override keyword was necessary to get this to work well with vec of parent class
        FixedObstacle(int w, int h) { // don't attempt an initializer list, caused a huge error from a huge int being stored in g_w and g_h previously
            grid_width = w;
            grid_height = h;
        } 
};

#endif