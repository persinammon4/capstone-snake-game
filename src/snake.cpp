#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  // this was pre-implemented for me
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Due to valid cell check in Controller, AI snake will never use wraparound

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  // AI snake has different death rules
  std::vector<SDL_Point> obstacle_points;
  SDL_Point p;

  // get a snapshot of all current obstacle points
  for (int i = 0; i < obstacles->size(); ++i) {
    auto obs = (*obstacles)[i].get();
    p.y = obs->leftMostPoint.y;
    for (int k = 0; k < obs->width; ++k) {
      p.x = obs->leftMostPoint.x + k;
      obstacle_points.push_back(p);
    }
  }

  // if snake (either fake or user controlled) runs into obstacle
  for (auto p : obstacle_points) {
    if (current_head_cell.x == p.x && current_head_cell.y == p.y) alive = false;
    for (auto p_body : body) {
      if (p_body.x == p.x && p_body.y == p.y) alive = false;
    }
  }

  if (!ai) {

    // if user snake runs into itself
    for (auto const &item : body) {
      if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
        alive = false;
      }
    }

    // if there exists a computer controlled snake, but still this is for
    // the user snake 
    if (fake_snake != nullptr) {
      // gather computer controlled snake current points in grid
      std::vector<SDL_Point> fake_snake_points;
      SDL_Point last_point;
      for (int i = 0; i < fake_snake->body.size(); ++i) {
        if (i == fake_snake->body.size()-1) {
          last_point = fake_snake->body[i];
        } else {
          fake_snake_points.push_back(fake_snake->body[i]);
        }
      }
      // check if there's a collision between user snake and computer controlled snake
      for (auto p : fake_snake_points) {
        for (auto p_me : body) {
          if (p_me.x == p.x && p_me.y == p.y) alive = false;
        }
        if (current_head_cell.x == p.x && current_head_cell.y == p.y) alive = false;
      }
      // a special edge case where user controlled snake is allowed to "eat" the computer controlled snake by hitting its tail
      // for any way the computer controlled snake dies (even if it hits an obstacle by accident), it stops moving and a 100 points are added to user score
      if (current_head_cell.x == last_point.x && current_head_cell.y == last_point.y) fake_snake->alive = false;
    }
  }
 
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}