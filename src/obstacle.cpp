#include "obstacle.h"

void MovingObstacle::Update() {
    switch (direction) {
        case Direction::kUp:
            // add case for if the wall is hit first
            if (leftMostPoint.y - 1 < 0 || path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
                // don't move
            } else {
                leftMostPoint.y -= 1;
                path_progress +=1;
            }
            break;
        case Direction::kDown:
            if (leftMostPoint.y + 1 < grid_height || path_progress == path_size) {
                direction = Direction::kUp;
                path_progress = 0;
            } else {
                leftMostPoint.y += 1;
                path_progress +=1;
            }
            break;
        case Direction::kLeft:
            if (leftMostPoint.x - 1 < 0 || path_progress == path_size) {
                direction = Direction::kRight;
                path_progress = 0;
            } else {
                leftMostPoint.x -= 1;
                path_progress +=1;
            }
            break;
        case Direction::kRight:
            if (leftMostPoint.x + 1 < grid_width || path_progress == path_size) {
                direction = Direction::kDown;
                path_progress = 0;
            } else {
                leftMostPoint.x += 1;
                path_progress +=1;
            }
            break;
        default:
            break;
    }
}

void FixedObstacle::Update() {return;}