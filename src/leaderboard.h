#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include<fstream>
#include<string>
#include<vector>
#include<tuple>
#include "game.h"

class LeaderBoard {

    public:
        LeaderBoard() {
            // if file doesn't exist, will create new ones
            // for renewing leaderboard, trusting in users having access
            // to delete the text files
            // 
            // use make_unique and perhaps emplace to create an array of smart pointers
            // to different files
        } 
    
        ~LeaderBoard() {
            // let file be using flush local
            // encrypt all
            // destroy all resources
        }


        // implement rule of 5 - declare copy and move constructors
        // although these aren't going to be used as LeaderBoard is called on stack by main.cpp
        // and always a high level object

        void addEntry(std::string username, int score, int size, GameObstacles obs_mode, GamesSnakes snake_mode);
        tuple<std::string, int> getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode);
        tuple<std::string, int> getGeneralRanking(std::string username);

        // all .txt files will be structured as:
        // username...score...size...timestamp/newlinecharacter
        // orders are updated during addEntry with local dictionary working
        std::string general_leaderboard_path{"leaderboards/general.txt"};
        std::string original_leaderboard_path{"leaderboards/original.txt"};
        std::string ai_leaderboard_path{"leaderboards/ai_controlled_two_player.txt"};
        std::string obstacle_leaderboard_path{"leaderboards/obstacles.txt"};
        std::string ai_obstacle_leaderboard_path{"leaderboards/ai_controlled_and_obstacles.txt"};

    private:
        // saves back in original location
        void encryptFile(std::string filename);
        void decryptFile(std::string filename, std::string out);

        // create and flush local dictionary in memory 
        void flushLocal();
        void loadLocal();
};

#endif