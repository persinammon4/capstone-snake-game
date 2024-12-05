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
#include <optional>
#include <mutex>

#include <iostream>
#include <../cryptopp/cryptlib.h>
#include <../cryptopp/rijndael.h>
#include <../cryptopp/modes.h>
#include <../cryptopp/files.h>
#include <../cryptopp/osrng.h>
#include <../cryptopp/hex.h>
#include <../cryptopp/aes.h>
#include <../cryptopp/pch.h>
#include <../cryptopp/filters.h>
#include <../cryptopp/ccm.h>

using namespace CryptoPP;


class LeaderBoard {

    // this is privately declared
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

    class LFile {
        std::filesystem::path encrypted_filename;
        std::filesystem::path decrypted_filename;

        public:
            LFile(std::filesystem::path encrypted_filename) : encrypted_filename(encrypted_filename) {}

            void addDecryptedPath(std::filesystem::path tmpfolder) {
                decrypted_filename = tmpfolder;
                decrypted_filename += encrypted_filename;
            }
            void addDecryptedFStream(std::fstream decryptstream) {
                //decryptstream = decryptstream;
            }
            std::filesystem::path read_encrypted_filename() { return encrypted_filename; }
            std::filesystem::path read_decrypted_filename() { return decrypted_filename; }
            //std::shared_ptr<std::fstream> decryptstream;
            std::fstream decryptstream;
    };


    public:
        LeaderBoard();

        // Rule of 5 implemented, although move and copy are unused currently by the usage in main.cpp (declared once on stack with no moves/copies)
        // decided this after modifying the destructor
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
        // left this public for external file name change
        std::filesystem::path general_leaderboard_path{"general.txt"};
        std::filesystem::path original_leaderboard_path{"original.txt"};
        std::filesystem::path ai_leaderboard_path{"ai_controlled_two_player.txt"};
        std::filesystem::path obstacle_leaderboard_path{"obstacles.txt"};
        std::filesystem::path ai_obstacle_leaderboard_path{"ai_controlled_and_obstacles.txt"};


    private:
        std::vector<LFile> data_files;
        std::vector<std::shared_ptr<std::fstream>> decrypted_files; // if copy the LeaderBoard, will copy shared_ptr instead of creating a new object

        // saves input file into a custom location or back to main folder
        // decided to call encrypt/decrypt in constructor/destructor instead of flush and load local data
        void encryptFile(LFile file, SecByteBlock key, SecByteBlock iv);
        void decryptFile(LFile file, SecByteBlock key, SecByteBlock iv); // will write to the decrypt path in LFile


        // create and flush local dictionary in memory 
        void flushLocal();
        void loadLocal();

        // helper methods
        std::optional<SecByteBlock> loadKeyFromFile();
        std::optional<SecByteBlock> loadIVFromFile();
        // getting all entries with the search username, can be optimized later
        std::vector<Entry> searchEntryVector(std::string username, std::vector<Entry> general_entries);
        void clearAndCreateNew(); // delete all encrypted data files and create a new key and iv

        
        std::filesystem::path main_folder{"leaderboards/"};
        
        std::filesystem::path key_path{"security_keys/key.txt"};
        // initialization random vector - needed to retrieve first block of block cipher during decryption
        std::filesystem::path iv_path{"security_keys/iv.txt"}; 

        // in memory local dictionary mapping leader board identification to vector of entries
        // simple file path to data relationship, no pre computed optimizations using other dictionaries
        // e.g. of an optimization is username to all entries, sorting by time_stamp
        std::map<std::string, std::vector<Entry>> data; 

        // use this to check for garbled text let's say if the file was tampered
        // or corruption spontaneously occured
        std::regex general_line_regex;

        std::filesystem::path tmppath;

        // mutex is to protect read and writes on the shared data (the encrypted files)
        // unnecessary right now but implemented in case of project extension
        // with multiple instances of Game sharing shared pointers to the same file streams/same files
        std::mutex file_rw_mutex;

        // decided not to store a class-level key and iv, even if it is private
        // instead within the functions they load the key and iv from files each time
};

#endif