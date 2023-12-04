#ifndef SCENARIO_H
#define SCENARIO_H

#include "game.h"

namespace Scenario {


    // it's not a simple input -> output, so instead of unit tests
    // have written test scenarios for manual snake testing
    // that will modify main.cpp to use

    // these double as real scenarios to feed into the game object
    // without having a randomized layout generator 
    // (where parameters are as simple as number of obstacles and type of obstacles etc.)

    void GameEnv0(Game &game) {
        // original, no additions
    }

    void GameEnv1(Game &game) { 
        // require that game given has the correct modes selected through static assert??
        // create fixed obstacles 

    }

    void GameEnv2(Game &game) {
        // create fake snake
    }

    void GameEnv3(Game &game) {
        // create moving obstacle
    }

    void GameEnv4(Game &game) {
        // create moving and fixed obstacles
    }

    void GameEnv5(Game &game) {
        // create fake snake and fixed obstacles
    }

    void GameEnv6(Game &game) {
        // create fake snake, fixed obstacles, moving obstacle
        // let chaos or buggy programs ensue!!
    }

    void DecideEnv(Game &game) {
        // try all game env's and pick the first one which works
        // or does GameEnv0 and moves on 
        // try-catch exceptions block
    }

}

#endif 