#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  constexpr std::size_t kFPSslow{40};
  constexpr std::size_t kFPSmedium{60};
  constexpr std::size_t kFPSfast{70};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);

  // launch a GUI
  // with options 
  // - see leaderboard, which is saved and written to a *.txt file
  // - decide on speed through checkboxes
  // - decide on original, obstacles (1,2,3), computer snake mode
  // - opt-in to leaderboard 

  // change kFramesPerSecond to one of the the 3 constexpr options
  const std::size_t kFramesPerSecond{60};
  const std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  game.Run(controller, renderer, kMsPerFrame);
  // save data to leaderboard (cleanup action)

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  // add leaderboard ranking to printout

  return 0;
}