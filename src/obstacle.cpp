#include "obstacle.h"

void MovingObstacle::Update() {
    switch (direction) {
        case Direction::kUp:
            // add case for if the wall is hit first
            leftMostPoint.y += 1; // how to add in speed?
            if (path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
            }
            break;
        case Direction::kDown:
            leftMostPoint.y -= 1; 
            if (path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
            }
            break;
        case Direction::kLeft:
            leftMostPoint.x -= 1;
            if (path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
            }
            break;
        case Direction::kRight:
            leftMostPoint.x += 1;
            if (path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
            }
            break;
        default:
            break;
    }
}

void FixedObstacle::Update() {return;}