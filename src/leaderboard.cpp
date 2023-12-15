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

std::vector<LeaderBoard::Entry> LeaderBoard::getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode) {
    if (obs_mode == GameObstacles::noObstacles) {
        if (snake_mode == GameSnakes::original) {
            return searchEntryVector(username, data[original_leaderboard_path]);
        }
        if (snake_mode == GameSnakes::computerSnake) {
            return searchEntryVector(username, data[ai_leaderboard_path]);
        }
    }
    if ((obs_mode == GameObstacles::fixedObstacles || obs_mode == GameObstacles::mixedObstacles)) {
        if (snake_mode == GameSnakes::original) {
            return searchEntryVector(username, data[obstacle_leaderboard_path]);
        }
        if (snake_mode == GameSnakes::computerSnake) {
            return searchEntryVector(username, data[ai_obstacle_leaderboard_path]);
        }
    }
    std::vector<Entry> entries;
    return entries;
}

// one day can change to an ordered vector with binary search + expansion for all entries with same username
// helper function
std::vector<LeaderBoard::Entry> LeaderBoard::searchEntryVector(std::string username, std::vector<Entry> general_entries) {
    std::vector<Entry> entries;
    for (Entry e : general_entries) {
        if (e.username == username) {
            entries.push_back(e);
        }
    }
    return entries;
} 

std::vector<LeaderBoard::Entry> LeaderBoard::getGeneralRanking(std::string username) {
    // load file if unloaded, else do not load and confuse versions
    return searchEntryVector(username, data[general_leaderboard_path]);
}
    
// void LeaderBoard::encryptFile(std::string filename) {


// }

// void LeaderBoard::decryptFile(std::string filename, std::string out) {

// }

// void LeaderBoard::flushLocal() {

// }

// void LeaderBoard::loadLocal() {
// if (std::regex_match(sting, regex)) { create an entry }
// }