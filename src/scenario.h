#ifndef SCENARIO_H
#define SCENARIO_H

#include "game.h"
#include "obstacle.h"
#include "snake.h"
#include <vector>
#include <stdexcept>
#include <functional>

namespace Scenario {


    // it's not a simple input -> output, so instead of unit tests
    // have written test scenarios for manual snake testing
    // that will modify main.cpp to use

    // these double as real scenarios to feed into the game object
    // without having a randomized layout generator 
    // (where parameters are as simple as number of obstacles and type of obstacles etc.)

    // throw logic error if modes aren't correct for scenario (invariant)

    // when placing objects, avoid the two locations where a fake snake and real snake would start
    // one square snakes - handled by game object


    // original, no additions
    void GameEnv0(Game &game) {
        // original, no additions
    }

    // create fixed obstacles
    void GameEnv1(Game &game) { 
        if (game.obstacle_mode == GameObstacles::noObstacles) throw std::logic_error("Wrong obstacle mode for GameEnv1 scenario");
        //game.addItem();
    }

    // create fake snake
    void GameEnv2(Game &game) {
        if (!(game.snake_mode == GameSnakes::computerSnake)) throw std::logic_error("Wrong snake mode for GameEnv2 scenario");

    }

    // create moving obstacles
    void GameEnv3(Game &game) {
        if (!(game.obstacle_mode == GameObstacles::mixedObstacles)) throw std::logic_error("Wrong obstacle mode for GameEnv3 scenario");


    }

    // create moving and fixed obstacles
    void GameEnv4(Game &game) {
        if (!(game.obstacle_mode == GameObstacles::mixedObstacles)) throw std::logic_error("Wrong obstacle mode for GameEnv4 scenario");

    }

    // create fake snake and fixed obstacles
    void GameEnv5(Game &game) {
        if (!(game.obstacle_mode == GameObstacles::fixedObstacles) || !(game.snake_mode == GameSnakes::computerSnake)) throw std::logic_error("Wrong obstacle and snake mode for GameEnv5 scenario");

    }

    // create fake snake, fixed obstacles, moving obstacle
    // let chaos and buggy programs ensue!!
    void GameEnv6(Game &game) {
        if (!(game.obstacle_mode == GameObstacles::mixedObstacles) || !(game.snake_mode == GameSnakes::computerSnake)) throw std::logic_error("Wrong obstacle and snake mode for GameEnv6 scenario");


    }

    // modify this with new function name when adding a new test case scenario
    //std::vector<std::function<void(Game)>> function_names_except_zero{GameEnv1, GameEnv2, GameEnv3, GameEnv4, GameEnv5, GameEnv6};
    std::vector<std::function<void(Game&)>> function_names_except_zero;

    // this driver method will try all game env's until one works
    // catch all case is GameEnv0
    void DecideEnv(Game &game) {
        function_names_except_zero.push_back(GameEnv1);
        function_names_except_zero.push_back(GameEnv2);
        function_names_except_zero.push_back(GameEnv3);
        function_names_except_zero.push_back(GameEnv4);
        function_names_except_zero.push_back(GameEnv5);
        function_names_except_zero.push_back(GameEnv6);
        for (auto func : function_names_except_zero) {
            try {
                func(game);
                break;
            } catch (...) {
                continue;
            }
        }
        GameEnv0(game);
    }

}

#endif 