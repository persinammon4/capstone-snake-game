#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include<fstream>
#include<string>
#include<vector>
#include "game.h"
#include <ctime>
#include <map>
#include <regex>


class LeaderBoard {

    struct Entry {
        std::string username;
        int score;
        int snake_size;
        std::time_t timestamp;

        Entry(std::string username, int score, int snake_size, std::time_t timestamp) : 
            username(username),
            score(score),
            snake_size(snake_size),
            timestamp(timestamp) {}

        std::string createString() {
            const time_t * temp_timestamp = &timestamp;
            return username + delimeter + std::to_string(score) +
                delimeter + std::to_string(snake_size) + 
                delimeter + ctime(temp_timestamp); // ctime will convert time_t to a c-style string; both functions are from C 
        }

        std::string createRegex() { 
            std::string escaped_delimeter;
            for (auto c : delimeter) escaped_delimeter += "\\" + c;
            std::string temp_regex("[A-Za-z]+" + escaped_delimeter + "[0-9]+" + escaped_delimeter + "[0-9]+" + escaped_delimeter + "[A-Za-z]{3} [A-Za-z]{3} [0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2} [0-9]{4}\n");
            return temp_regex;
        } 


        private:
            std::string delimeter{"..."};
    };

    public:
        LeaderBoard() {

            // this looks odd, but it allows for the delimeter to remain private instead of a public read
            // and for the entire write pattern of data to be stored in one location (the Entry struct above) 
            // this prevents two definitions of the format, so it's impossible for there to be a programmer error
            Entry temp_e("", 0, 0, std::time(nullptr));
            std::regex line_regex(temp_e.createRegex());
            general_line_regex = line_regex;

            // for each encrypted path
            // hold a lock onto the file
            // if the file doesn't exist, 
            // create an empty file and an empty decrypted temp file (with a separate lock on it)
            // rename the empty decrypted file if it's already made by another LeaderBoard
            // save make_unique resource and emplace to a vector
            // and load the contents into the data var
            // release the lock
            for (auto p : encrypted_paths) {
                // hold a lock on a file
                // do actions
                // release lock
            }
            
        } 
    
        ~LeaderBoard() {
            // for all data
            // flush local using another lock on the file
            // encrypt all using a lock on the file
            // release the locks
            // delete data
            // destroy any manually made by new keyword resource
        }


        // implement rule of 5 - declare copy and move constructors
        // although these aren't going to be used as LeaderBoard is called on stack by main.cpp
        // and always a high level object


        void addEntry(std::string username, int score, int size, GameObstacles obs_mode, GameSnakes snake_mode);

        std::vector<Entry> getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode);
        std::vector<Entry> getGeneralRanking(std::string username);

        // all .txt files will be structured as:
        // username...score...snakesize...timestamp/newlinecharacter
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

        // decrypted files are temp files with paths stored here 
        std::vector<std::string> decrypted_paths;
        
        std::vector<std::string> encrypted_paths{general_leaderboard_path, original_leaderboard_path,
        ai_leaderboard_path, obstacle_leaderboard_path, ai_obstacle_leaderboard_path};

        // in memory local dictionary mapping leader board identification to vector of entries
        // simple file path to data relationship, no pre computed optimizations using other dictionaries
        // e.g. username to all entries, sorting by time_stamp
        std::map<std::string, std::vector<Entry>> data; 

        // use this to check for garbled text let's say if the file was tampered
        // or corruption spontaneously occured
        std::regex general_line_regex;

        // locks are to protect read and writes - let's say a user on the 
        // same machine sees the intermediary temp files and or the encrypted files
        // and attempts to modify it. Or, if later on another LeaderBoard class tries to write to the 
        // same paths by accident
};

#endif