#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include<fstream>
#include<string>
#include<vector>
#include "game.h"
#include <ctime>
#include <map>
#include <regex>
#include <filesystem>

#include <iostream>
#include <iomanip>
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

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
                delimeter + ctime(temp_timestamp); // ctime will convert time_t to a c-style string; both time_t and ctime are from C 
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
        LeaderBoard();

        // Rule of 5 implemented, although move and copy are unused currently by the usage in main.cpp (declared once on stack with no moves/copies)
        ~LeaderBoard();
    
        // copy constructor and assignment operator
        LeaderBoard(const LeaderBoard &source);
        LeaderBoard& operator=(const LeaderBoard &source);

        // move constructor and assignment operator
        LeaderBoard(LeaderBoard &&source);
        LeaderBoard& operator=(LeaderBoard &&source);


        void addEntry(std::string username, int score, int size, GameObstacles obs_mode, GameSnakes snake_mode);

        std::vector<Entry> getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode);
        std::vector<Entry> getGeneralRanking(std::string username);

        // all .txt files will be structured as:
        // username...score...snakesize...timestamp/newlinecharacter
        // orders are updated during addEntry with local dictionary working
        std::filesystem::path general_leaderboard_path{"general.txt"};
        std::filesystem::path original_leaderboard_path{"original.txt"};
        std::filesystem::path ai_leaderboard_path{"ai_controlled_two_player.txt"};
        std::filesystem::path obstacle_leaderboard_path{"obstacles.txt"};
        std::filesystem::path ai_obstacle_leaderboard_path{"ai_controlled_and_obstacles.txt"};


    private:
        // saves input file into a custom location or back to main folder
        // decided to call encrypt/decrypt in constructor/destructor instead of flush and load local data
        void encryptFile(std::string filename);
        void decryptFile(std::string filename, std::string out);

        // create and flush local dictionary in memory 
        void flushLocal();
        void loadLocal();

        // helper method for getting all entries with the search username, can be optimized later
        std::vector<Entry> searchEntryVector(std::string username, std::vector<Entry> general_entries);


        // decrypted files are temp files with paths stored here 
        // use shared_ptr for potentially multiple LeaderBoards using the same init created fstream
        std::vector<std::shared_ptr<std::fstream>> decrypted_files;
        
        std::vector<std::filesystem::path> encrypted_paths;

        std::filesystem::path main_folder{"leaderboards/"};
        
        std::filesystem::path key_path{"security_keys/key.txt"};


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