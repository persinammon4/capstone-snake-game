#include <iostream>
#include <string>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "leaderboard.h"
#include "scenario.h"



int main() {
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  constexpr std::size_t kFPSslow{40};
  constexpr std::size_t kFPSmedium{60};
  constexpr std::size_t kFPSfast{70};

  std::string username{"anonymous"}; //default value, should use heap?

  LeaderBoard leaderboard;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);




  auto obstacle_mode = GameObstacles::mixedObstacles;
  auto snake_mode = GameSnakes::computerSnake;
  auto speed_mode = GameSpeeds::medium;

  // launch a GUI


  // these are the enum definitions in game.h
  // enum class GameSpeeds { slow, medium, fast};
  // enum class GameObstacles { fixedObstacles, movingObstacles, noObstacles };
  // enum class GameSnakes { original, computerSnake };
  // not happy with noObstacles == noob...
  Game game(kGridWidth, kGridHeight, speed_mode, obstacle_mode, snake_mode);


  // change kFramesPerSecond to one of the the 3 constexpr options
  std::size_t kFramesPerSecond;

  // slight sanity check by using the game's speed_mode
  switch(game.speed_mode) {
    case GameSpeeds::slow:
      kFramesPerSecond = kFPSslow;
      break;
    case GameSpeeds::medium:
      kFramesPerSecond = kFPSmedium;
      break;
    case GameSpeeds::fast:
      kFramesPerSecond = kFPSfast;
      break;
    default:
      kFramesPerSecond = kFPSmedium;
  }

  const std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  //populate Game with different scenario environments
  Scenario::DecideEnv(game);

  // take concurrency into account if on the heap


  game.Run(renderer, kMsPerFrame); // start game loop

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  //leaderboard.addEntry(username, game.getScore(), game.getSize(), game.obstacle_mode, game.snake_mode);
  std::cout << "Added entry to leaderboards.\n";
  //leaderboard.getSpecificRanking(username, type of );
  //leaderboard.getGeneralRanking(username);
  // add leaderboard ranking to printout
  //std::cout << leaderboard.getRanking(username, type of leaderboard);
  return 0;
}