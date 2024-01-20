#include "BlackListHandler.h"
#include "errors/InternalServerError.h"
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

std::mutex BlackListHandler::file_mutex;

const std::string BlackListHandler::BAN_DIRECTORY = "./blacklist";

bool BlackListHandler::is_banned(const std::string &user_name, const std::string &client_address) {
    return get_bans_amount(user_name, client_address) == 3;
}

void BlackListHandler::ban(const std::string &user_name, const std::string &client_address) {
    std::lock_guard<std::mutex> lock(file_mutex);
    try {
        std::string ban_file_path = get_user_ban_file(user_name, client_address);
        std::ofstream file_stream(ban_file_path, std::ios::trunc);
        if (!file_stream.is_open()) throw InternalServerError("Internal Server Error: Failed to open ban file for writing...");

        int current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        file_stream << "3\n" << current_time << std::endl;
        file_stream.close();

        std::thread([ban_file_path]() {
            std::this_thread::sleep_for(std::chrono::seconds(60));
            if (fs::exists(ban_file_path)) {
                fs::remove(ban_file_path);
            }
        }).detach();

    } catch (const std::ofstream::failure &exception) {
        throw InternalServerError("Internal Server error : failed to write ban to the file...");
    }
}


void BlackListHandler::increase_wrong_tries(const std::string &user_name, const std::string &client_address) {
    std::lock_guard<std::mutex> lock(file_mutex);
    try {
        if (!fs::exists(BAN_DIRECTORY)) fs::create_directory(BAN_DIRECTORY);
        int bans_amount = get_bans_amount(user_name, client_address);

        bans_amount++;
        std::ofstream file_stream(get_user_ban_file(user_name, client_address));
        if (!file_stream.is_open()) throw InternalServerError("Internal Server Error: Failed to open ban file for increasing bans amount...");
        file_stream << bans_amount;
        file_stream.close();
    } catch (const std::ofstream::failure &) {
        throw InternalServerError("Internal Error : failed to write increased bans amount to the file...");
    }
}

std::string BlackListHandler::get_user_ban_file(const std::string &user_name, const std::string &client_address) {
    size_t colon_pos = client_address.find(':');
    std::string address_part = client_address.substr(0, colon_pos);
    return BAN_DIRECTORY + "/" + user_name + "#" + address_part;
}


int BlackListHandler::get_bans_amount(const std::string &user_name, const std::string &client_address) {
    try {
        std::string file_path = get_user_ban_file(user_name, client_address);

        if (!fs::exists(file_path)) return 0;

        std::ifstream file_stream(file_path);
        if (!file_stream.is_open()) throw InternalServerError("Internal Server Error: Failed to open ban file for reading...");

        std::string line;
        std::getline(file_stream, line);
        file_stream.close();

        return std::stoi(line);
    } catch (const std::ifstream::failure &exception) {
        throw InternalServerError("Internal Server error : failed to obtain bans amount...");
    }
}

void BlackListHandler::check_old_bans() {
    try {
        if (!fs::exists(BAN_DIRECTORY)) return;

        for (const auto &entry : fs::directory_iterator(BAN_DIRECTORY)) {
            std::ifstream file_stream(entry.path());
            if (!file_stream.is_open()) continue;

            std::string bans_str, timestamp_str;
            std::getline(file_stream, bans_str);
            std::getline(file_stream, timestamp_str);
            file_stream.close();

            int bans = std::stoi(bans_str);
            int timestamp = std::stoi(timestamp_str);

            int current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            if (bans == 3 && (current_time - timestamp) > 60) {
                fs::remove(entry.path());
            }
        }
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : failed to check old bans...");
    }
}

