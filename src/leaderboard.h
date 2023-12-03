#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include<fstream>
#include<string>
#include<vector>

enum class CREATE_MODE { create = true, skip = false};

class LeaderBoard {

    public:
        LeaderBoard(CREATE_MODE create_mode = CREATE_MODE::create) { // if should have a specific option parameter
            // if file doesn't exist, or create new leaderboard
            // if (create_mode || "file doesnt exist") {
            //     // create a new text file called general_leaderboard, options_leaderboard
            // } else {
            
            // }
        }
        ~LeaderBoard() {
            // let file be saved
            // rule that a name can only appear 3 times
        }
        // implement rule of 5 - copy and move constructors

        // use make_unique and perhaps emplace to create an array of smart pointers
        // to different files


    private:
        void encryptFile(std::string filename);
        void decryptFile(std::string filename);
};

#endif