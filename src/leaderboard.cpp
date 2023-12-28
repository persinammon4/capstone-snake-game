#include "leaderboard.h"


LeaderBoard::LeaderBoard() {

    data_files.emplace_back(general_leaderboard_path);
    data_files.emplace_back(original_leaderboard_path);
    data_files.emplace_back(ai_leaderboard_path);
    data_files.emplace_back(obstacle_leaderboard_path);
    data_files.emplace_back(ai_obstacle_leaderboard_path);

    // may need to populate data map with empty keys, check later

    // if no leaderboards then create leaderboards
    if (!std::filesystem::exists(main_folder)) std::filesystem::create_directory(main_folder);

    tmppath = std::filesystem::temp_directory_path();

    for (auto &f : data_files) { // reference avoids a redundant copy (?)
        f.addDecryptedPath(tmppath);
    }

    // Load key and iv
    // if either key or iv is invalid, then clear everything
    // for a future extension, if only iv is unrecoverable, can attempt to recover all data outside of the first block in the block cipher
    // block size is in bits/bytes
    if (!std::filesystem::exists(key_path) || !std::filesystem::exists(iv_path)) {
        clearAndCreateNew();
    }

    // this will either load in pre-existing key and iv or new key and iv 
    // if new key and iv, it's ok because the old encrypted files were deleted
    // and the for loop will iterate over nothing
    if (std::filesystem::exists(key_path) && std::filesystem::exists(iv_path)) {

        SecByteBlock key (AES::DEFAULT_KEYLENGTH);
        SecByteBlock iv(AES::BLOCKSIZE);

        auto throwaway_optional = loadKeyFromFile();
        if (throwaway_optional.has_value()) { // throw away everything if invalid
            key = throwaway_optional.value();
        } else {
            clearAndCreateNew();
        }

        throwaway_optional = loadIVFromFile();
        if (throwaway_optional.has_value()) { // throw away everything if invalid
            iv = throwaway_optional.value();
        } else {
            clearAndCreateNew();
        }

        // let's assume everything went well, and there are encrypted files to decrypt
        // if something failed, there would be no encrypted files to decrypt
        // because all deleted in clear and create new func
        for (auto &f : data_files) {
            if (std::filesystem::exists(f.read_decrypted_filename())) {
                // will lock during the decrypt
                //decryptFile(f, key, iv);
                std::fstream decryptstream(f.read_decrypted_filename());
                //std::shared_ptr<std::fstream> decrypt_ptr = std::make_shared<std::fstream>(decryptstream);
                //decrypted_files.push_back(decrypt_ptr);
                //f.addDecryptedFStream(decrypt_ptr); // creates a copy
            }
        }

        // load in all decrypted files
        loadLocal(); 
    } else {
        // this will never be triggered due to prev if statement, but added it in anyway
        clearAndCreateNew();
    }

    // ends with fully loaded abstract data structures handling information retrieval
    // whether that is empty or populated by the above logic, and decrypted files in tmppath
    // ends in the main folder being created if doesn't exist
    // ends in a working key and iv
}

LeaderBoard::~LeaderBoard() {
    flushLocal(); // saves data to decrypted file then clears data dictionary
    // using the decrypted file instead of direct write to encrypted file

    // for the encryption step, either use pre-existing key and iv
    // or create a new one now
    SecByteBlock key (AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    auto throwaway_optional = loadKeyFromFile();
    if (throwaway_optional.has_value()) { 
        key = throwaway_optional.value();
    } else {
        // create and save a new key
        AutoSeededRandomPool prng;
        SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        prng.GenerateBlock(key, key.size());
    }

    throwaway_optional = loadIVFromFile();
    if (throwaway_optional.has_value()) { 
        iv = throwaway_optional.value();
    } else {
        // create and save a new iv
        AutoSeededRandomPool prng;
        SecByteBlock iv(AES::BLOCKSIZE);
        prng.GenerateBlock(iv, iv.size());
    }

    for (auto &f : data_files) { 
        std::string plain;
        //f.decryptstreamopen();
        // create an f stream out of the decrypted path
        // read that into plain string  
        //f.decryptstream->close();
        //encryptFile(f, key, iv); // encrypt the file given the two paths, key and iv loaded in
        std::filesystem::remove(f.read_decrypted_filename());
    }
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

// later can change to an ordered vector with binary search + expansion for all entries with same username
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


void LeaderBoard::encryptFile(LFile file, SecByteBlock key, SecByteBlock iv) {

    std::lock_guard<std::mutex> lockg(file_rw_mutex);

    AutoSeededRandomPool prng;
    const char* encrypt_file_sink = file.read_encrypted_filename().generic_string().c_str();
    HexEncoder encoder(new FileSink(encrypt_file_sink, true)); 
    
    std::string cipher;

    try {
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        // this was probably changed, go redo it
        std::string plain;
        // create a string from whatever is in LFile decrypt path
        StringSource s(plain, true, 
            new StreamTransformationFilter(e, new StringSink(cipher)));

    } catch (const Exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    // key
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std:: cout << std::endl;

    // iv
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    // cipher text, key and iv already set in encoderm this cipher text needs
    // to be streamed into a file
    //encoder.Put((), cipher.size());
    //encoder.MessageEnd((const byte*)&cipher[0], cipher.size());
    std::cout << std::endl;
}
    

void LeaderBoard::decryptFile(LFile file, SecByteBlock key, SecByteBlock iv) {

    // lock on file operation
    std::lock_guard<std::mutex> lockg(file_rw_mutex);

    std::string recovered;
    std::string cipher;

    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true, 
            new StreamTransformationFilter(d, new StringSink(recovered)));

        // recovered text
        std::cout << recovered << std::endl;
        
    } catch (const Exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::optional<SecByteBlock> LeaderBoard::loadKeyFromFile() {
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);

    std::ifstream read_file; // fstream variable, destructor will close file automatically on leave of scope

    std::lock_guard<std::mutex> lockg(file_rw_mutex);

    // load key from file
    read_file.open(key_path.generic_string()); // connect to a stream object
    if (read_file.is_open()) {
        while (read_file) { 
            // assume there's no whitespace or whitespace is ruining key
            read_file >> key;
            // if there is a new line and want to get line by line separated by \n, without it
            // automatically stripping \n like the above command, use file.get();
        }
    }
    read_file.close();
    // if key not valid, return std::nullopt for the optional
    return key;
}

std::optional<SecByteBlock> LeaderBoard::loadIVFromFile() {
    SecByteBlock iv(AES::BLOCKSIZE);

    std::ifstream read_file;

    std::lock_guard<std::mutex> lockg(file_rw_mutex);


    // load iv from file
    read_file.open(iv_path.generic_string());
    if (read_file.is_open()) {
        while (read_file) {
            read_file >> iv;
        }
    }
    read_file.close();
    // if iv not valid, return std::nullopt for the optional
    return iv;
}

void LeaderBoard::clearAndCreateNew() {

    // have a lock guard here 
    std::lock_guard<std::mutex> lockg(file_rw_mutex);


    std::filesystem::remove_all(main_folder);  
    std::filesystem::remove(key_path);
    std::filesystem::remove(iv_path);

    std::filesystem::create_directories(main_folder); // cleared the files when shown to be invalid

    // create a new iv and key
    AutoSeededRandomPool prng;
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    // save iv and key to files
    // Create StringSink

    std::ofstream iv_writer(iv_path.generic_string());
    //iv_writer << iv << std::endl;
    iv_writer.close();

    std::ofstream key_writer(key_path.generic_string());
    key_writer.close();
}

void LeaderBoard::flushLocal() {

    std::lock_guard<std::mutex> lockg(file_rw_mutex);

    for (auto &f : data_files) {
        // write to decrypted path file
        std::string plain;
        //for (Entry e : data[key]) {
            //plain << e.getString();
        //}
        //f.decryptstream << plain;
    }
    data.clear(); // delete local data
}

void LeaderBoard::loadLocal() {

    std::lock_guard<std::mutex> lockg(file_rw_mutex);

    // have all unencrypted tmp files and parse them, adding entries to data
    // this looks odd, but it allows for the delimeter to remain private instead of a public read
    // and for the entire write pattern of data to be stored in one location (the Entry struct above) 
    // this prevents two definitions of the format, so it's impossible for there to be a programmer error
    Entry temp_e("", 0, 0, std::time(nullptr));
    std::regex line_regex(temp_e.createRegex());
    general_line_regex = line_regex;
    // for (auto &f : data_files) {
    //     f.decrypstream->open();
    //     while() {
    //         std::string l = f.decryptstream.get_line();
    //         if (std::regex_match(l, regex)) {
    //             data[].emplace_back(entry parameters);
    //         }
    //         // else if garbled will skip to the next new line spaced line
    //     }
    // }
}