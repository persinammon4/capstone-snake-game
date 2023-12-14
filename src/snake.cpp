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
  // in their algorithm direction finder
  std::vector<SDL_Point> obstacle_points;


  for (int i = 0; i < obstacles->size(); ++i) {
    SDL_Point p;
    auto obs = (*obstacles)[i].get();
    p.y = obs->leftMostPoint.y;
    for (int k = 0; k < obs->width; ++k) {
      p.x = obs->leftMostPoint.x + k;
      obstacle_points.push_back(p);
    }
  }

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

    // if there exists a computer controlled snake
    if (fake_snake != nullptr) {

      auto body_except_last = fake_snake->body; 

      int new_x_fake = static_cast<int>(fake_snake->head_x);
      int new_y_fake = static_cast<int>(fake_snake->head_y);

      // hit head of fake snake
      if (current_head_cell.x == new_x_fake && current_head_cell.y == new_y_fake) alive = false;
      // and there is no body, so no need to check further (avoids segmentation fault from iterating over empty vec)
      if (body_except_last.size() == 0) return; 
      
      // last point is going to have a win scenario for player snake
      size_t s = body_except_last.size()-1;
      SDL_Point last_point = body_except_last[s];
      body_except_last.pop_back();

      if (current_head_cell.x == last_point.x && current_head_cell.y == last_point.y) {
        fake_snake->alive = false;
        // game score increases += 100 in the parent game, indiscriminately after computer snake death
      }
      for (auto p_body : body ) {
        if (p_body.x == last_point.x && p_body.y == last_point.y) fake_snake->alive = false;
        // same game score boost etc as previous if statement
      }

      // collision logic against the fake snake body
      for (auto const &item : body_except_last) {
        if (current_head_cell.x == item.x && current_head_cell.y == item.y) alive = false;
        for (auto p_body : body) {
          if (p_body.x == item.x && p_body.y == item.y) alive = false;
        }
      }
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