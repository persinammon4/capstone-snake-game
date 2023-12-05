#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "obstacle.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  // overload Render function for different game modes
  void Render(Snake const snake, SDL_Point const &food); // original
  void Render(Snake const snake, SDL_Point const &food, std::vector<Obstacle *> const obstacles); // obstacles
  void Render(Snake const snake, SDL_Point const &food, Snake const fake_snake); // competitor snake
  void Render(Snake const snake, SDL_Point const &food, std::vector<Obstacle *> const obstacles, Snake const fake_snake); // competitor snake and obstacles

  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif