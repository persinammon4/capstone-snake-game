#include "leaderboard.h"

// the string path handles are as follows:
// general_leaderboard_path
// original_leaderboard_path
// ai_leaderboard_path
// obstacle_leaderboard_path
// ai_obstacle_leaderboard_path

void LeaderBoard::addEntry(std::string username, int score, int size, GameObstacles obs_mode, GameSnakes snake_mode) {
    // no such thing as switch statement with two parameters
    Entry e(username, score, size, std::time(nullptr));
    if (obs_mode == GameObstacles::noObstacles) {
        if (snake_mode == GameSnakes::original) {
            data[original_leaderboard_path].push_back(e);
        } else if (snake_mode == GameSnakes::computerSnake) {
            data[ai_leaderboard_path].push_back(e);
        }

    } else if (obs_mode == GameObstacles::fixedObstacles || obs_mode == GameObstacles::mixedObstacles) {
        if (snake_mode == GameSnakes::original) {
            data[obstacle_leaderboard_path].push_back(e);
        } else if (snake_mode == GameSnakes::computerSnake) {
            data[ai_obstacle_leaderboard_path].push_back(e);
        }
    } 
    data[general_leaderboard_path].push_back(e);

    // will save back to files during a later call to flushLocal
}

// std::vector<Entry> LeaderBoard::getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode) {

// }

// std::vector<Entry> LeaderBoard::getGeneralRanking(std::string username) {
//     std::vector<Entry> entries;
//     // load file if unloaded, else do not load and confuse versions

//     for (int i = 0; i < )
// }
    
// void LeaderBoard::encryptFile(std::string filename) {


// }

// void LeaderBoard::decryptFile(std::string filename, std::string out) {

// }

// void LeaderBoard::flushLocal() {

// }

// void LeaderBoard::loadLocal() {

// }