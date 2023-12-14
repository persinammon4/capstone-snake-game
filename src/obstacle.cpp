#include "obstacle.h"
#include <iostream>

void MovingObstacle::Update() {
    switch (direction) {
        case Direction::kUp:
            if (path_progress >= path_size* denominator_slowdown) {
                direction = Direction::kDown;
                path_progress = 0;
                break;
            }
            path_progress += 1;
            if (leftMostPoint.y - 1 > 0 && path_progress % denominator_slowdown == 0) {
                leftMostPoint.y -= 1;
            } else if (leftMostPoint.y - 1 <= 0)  {
                direction = Direction::kDown;
                path_progress = 0;
                break;
            }
            break;
        case Direction::kDown:
            if (path_progress >= path_size* denominator_slowdown) {
                direction = Direction::kUp;
                path_progress = 0;
                break;
            }
            path_progress += 1;
            // grid_height - 1 is the last entry, assuming it's zero-indexed
            if (leftMostPoint.y + 1 < grid_height-1 && path_progress % denominator_slowdown == 0) {
                leftMostPoint.y += 1;
            } else if (leftMostPoint.y - 1 <= 0)  {
                direction = Direction::kUp;
                path_progress = 0;
                break;
            }
            break;
        case Direction::kLeft:
            if (path_progress >= path_size* denominator_slowdown) {
                direction = Direction::kRight;
                path_progress = 0;
                break;
            }
            path_progress += 1;
            if (leftMostPoint.x - 1 > 0 && path_progress % denominator_slowdown == 0) {
                leftMostPoint.x -= 1;
            } else if (leftMostPoint.x - 1 <= 0)  {
                direction = Direction::kRight;
                path_progress = 0;
                break;
            }
            break;
        case Direction::kRight:
            if (path_progress >= path_size*denominator_slowdown) {
                direction = Direction::kLeft;
                path_progress = 0;
                break;
            }
            path_progress += 1;
            // grid_width - 1 is the last entry, assuming it's zero-indexed
            if (leftMostPoint.x + 1 < grid_width-1 && path_progress % denominator_slowdown == 0) {
                leftMostPoint.x += 1;
            } else if (leftMostPoint.x + 1 >= grid_width) {
                direction = Direction::kLeft;
                path_progress = 0;
                break;
            }
            break;
        default:
            break;
    }
}

void FixedObstacle::Update() {}