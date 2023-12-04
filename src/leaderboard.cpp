#include "leaderboard.h"
#include<tuple>

// the string path handles are as follows:
// general_leaderboard_path
// original_leaderboard_path
// ai_leaderboard_path
// obstacle_leaderboard_path
// ai_obstacle_leaderboard_path

void LeaderBoard::addEntry(std::string username, int score, int size) {

}

std::tuple<std::string, int> LeaderBoard::getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode) {

}

std::tuple<std::string, int> LeaderBoard::getGeneralRanking(std::string username) {

}
    
void LeaderBoard::encryptFile(std::string filename) {


}

void LeaderBoard::decryptFile(std::string filename, std::string out) {

}

void LeaderBoard::flushLocal() {

}

void LeaderBoard::loadLocal() {

}