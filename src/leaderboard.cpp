#include "leaderboard.h"




LeaderBoard::LeaderBoard() {
    // // if no leaderboards then create leaderboards
    // if (!std::filesystem::exists(main_folder)) std::filesystem::create_directory(main_folder);

    // // if already leaderboards/tmp then clean up leaderboards/tmp
    // std::filesystem::path tmp_path = main_folder + "tmp/";
    // if (std::filesystem::exists(tmp_path)) std::filesystem::remove_all(tmp_path);

    // // Using symmetric AES encryption
    // // Load key or keys, if nothing works then delete encrypted files and create new empty files
    // // Assume without a key the data is irrecoverable
    // // load content of key.txt then      
    // if (key) {
    //     for (auto &p : encrypted_paths) {
    //         // decryptFile(p, );
    //         // create a shared_ptr
    //     }
    //     loadLocal(); // uses a lock for read
    // } else {
    //     std::filesystem::remove_all(main_folder);
    //     // create new empty encrypted files
    //     // create a shared_ptr
    // }
}

LeaderBoard::~LeaderBoard() {
    // flushLocal(); 
    // for (auto &p : decrypted_paths) {
    //     encryptFile(p);
    // }
    // std::filesystem::remove_all("leaderboards/tmp");
}


// copy constructor and assignment operator
// LeaderBoard::LeaderBoard(const LeaderBoard &source) {

// }
// LeaderBoard& LeaderBoard::operator=(const LeaderBoard &source) {

// }

// // move constructor and assignment operator
// LeaderBoard::LeaderBoard(LeaderBoard &&source) {

// }
// LeaderBoard& LeaderBoard::operator=(LeaderBoard &&source) {

// }

// the string path handles are as follows:
// general_leaderboard_path
// original_leaderboard_path
// ai_leaderboard_path
// obstacle_leaderboard_path
// ai_obstacle_leaderboard_path

void LeaderBoard::addEntry(std::string username, int score, int size, GameObstacles obs_mode, GameSnakes snake_mode) {
    // no such thing as switch statement with two parameters
    if (obs_mode == GameObstacles::noObstacles) {
        if (snake_mode == GameSnakes::original) {
            data[original_leaderboard_path].emplace_back(username, score, size, std::time(nullptr));
        } else if (snake_mode == GameSnakes::computerSnake) {
            data[ai_leaderboard_path].emplace_back(username, score, size, std::time(nullptr));
        }

    } else if (obs_mode == GameObstacles::fixedObstacles || obs_mode == GameObstacles::mixedObstacles) {
        if (snake_mode == GameSnakes::original) {
            data[obstacle_leaderboard_path].emplace_back(username, score, size, std::time(nullptr));
        } else if (snake_mode == GameSnakes::computerSnake) {
            data[ai_obstacle_leaderboard_path].emplace_back(username, score, size, std::time(nullptr));
        }
    } 
    data[general_leaderboard_path].emplace_back(username, score, size, std::time(nullptr));

    // will save back to files during a later call to flushLocal
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

// the string path handles are as follows:
// general_leaderboard_path
// original_leaderboard_path
// ai_leaderboard_path
// obstacle_leaderboard_path
// ai_obstacle_leaderboard_path

std::vector<LeaderBoard::Entry> LeaderBoard::getSpecificRanking(std::string username, GameObstacles obs_mode, GameSnakes snake_mode) {
    if (obs_mode == GameObstacles::noObstacles && snake_mode == GameSnakes::original) return searchEntryVector(username, data[original_leaderboard_path]);
    if ((obs_mode == GameObstacles::fixedObstacles || obs_mode == GameObstacles::mixedObstacles ) && snake_mode == GameSnakes::original) return searchEntryVector(username, data[obstacle_leaderboard_path]);
    if (obs_mode == GameObstacles::noObstacles && snake_mode == GameSnakes::computerSnake) return searchEntryVector(username, data[ai_leaderboard_path]);
    if ((obs_mode == GameObstacles::fixedObstacles || obs_mode == GameObstacles::mixedObstacles )&& snake_mode == GameSnakes::computerSnake) return searchEntryVector(username, data[ai_obstacle_leaderboard_path]);
    return getGeneralRanking(username);
}

    
void LeaderBoard::encryptFile(std::string filename) {
    // AES

}

void LeaderBoard::decryptFile(std::string filename, std::string out) {
    // AES
}


void LeaderBoard::flushLocal() {
    // // create unencrypted tmp files if not exist or or write to preexisting tmp files
    // for (std::filesystem::path p : encrypted_paths) {
    //     if (!std::filesystem::exists(p)) {
    //         // create new file
    //         // write to new file
    //     } else {
    //         // clear contents of file
    //         // write to new file
    //     }
    //     // delete tmp file
    // } 
    // data.clear(); // delete local data
}

void LeaderBoard::loadLocal() {
    // have all unencrypted tmp files and parse them, adding entries to data
    // this looks odd, but it allows for the delimeter to remain private instead of a public read
    // and for the entire write pattern of data to be stored in one location (the Entry struct above) 
    // this prevents two definitions of the format, so it's impossible for there to be a programmer error
    // Entry temp_e("", 0, 0, std::time(nullptr));
    // std::regex line_regex(temp_e.createRegex());
    // general_line_regex = line_regex;
    // for (auto &f : decrypted_files) {
    //     for (f.get_line()) {
    //         if (std::regex_match(string, regex)) {
    //             // get correct path for right data in vec
    //             data[].emplace_back(); // create an entry
    //         }
    //     }
    // }
}