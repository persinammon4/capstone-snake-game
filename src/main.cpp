#include <iostream>
#include <string>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#define NK_IMPLEMENTATION
#include "nuklear.h"
#include "leaderboard.h"
#include "scenario.h"

#define MAX_MEMORY (1024 * 1024 * 2) /* 2MB of memory */


int main() {
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  constexpr std::size_t kFPSslow{40};
  constexpr std::size_t kFPSmedium{60};
  constexpr std::size_t kFPSfast{70};

  std::string username{"anonymous"}; //default value

  LeaderBoard leaderboard;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;


  // launch a GUI
  // with options 
  // - see leaderboard, which is saved and written to a *.txt file
  // - decide on speed through checkboxes
  // - decide on original, obstacles (1,2,3), computer snake mode
/* init gui state */
// struct nk_context ctx;
// nk_user_font *font = nkc_load_font_file(&atlas, "./extra_font/DroidSans.ttf", 13, 0);

// nk_init_fixed(&ctx, calloc(1, MAX_MEMORY), MAX_MEMORY, &font);

// enum {EASY, HARD};
// static int op = EASY;
// static float value = 0.6f;
// static int i =  20;

// if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
//     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
//     /* fixed widget pixel width */
//     nk_layout_row_static(&ctx, 30, 80, 1);
//     if (nk_button_label(&ctx, "button")) {
//         /* event handling */
//     }

//     /* fixed widget window ratio width */
//     nk_layout_row_dynamic(&ctx, 30, 2);
//     if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
//     if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;

//     /* custom widget pixel width */
//     nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
//     {
//         nk_layout_row_push(&ctx, 50);
//         nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
//         nk_layout_row_push(&ctx, 110);
//         nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
//     }
//     nk_layout_row_end(&ctx);
// }
// nk_end(&ctx); // called at end of window buildiing process for general cleanup and scaling, scrollbars etc

  auto obstacle_mode = GameObstacles::mixedObstacles;
  auto snake_mode = GameSnakes::computerSnake;
  auto speed_mode = GameSpeeds::medium;

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


  game.Run(controller, renderer, kMsPerFrame); // start game loop

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  //leaderboard.addEntry(username, game.getScore(), game.getSize(), game.obstacle_mode, game.snake_mode);
  std::cout << "Added entry to leaderboards.\n";
  //leaderboard.getSpecificRanking(username, type of );
  //leaderboard.getGeneralRanking(username);
  // add leaderboard ranking to printout
  //std::cout << leaderboard.getRanking(username, type of leaderboard);
  //nk_font_atlas_clear(&atlas);
  return 0;
}