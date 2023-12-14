#include "BlackListHandler.h"
#include "errors/InternalServerError.h"
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

const std::string BlackListHandler::BAN_DIRECTORY = "./blacklist";

bool BlackListHandler::is_banned(const std::string &user_name, const std::string &client_address) {
    return get_bans_amount(user_name, client_address) == 3;
}

void BlackListHandler::ban(const std::string &user_name, const std::string &client_address) {
    std::thread([user_name, client_address]() {
        std::this_thread::sleep_for(std::chrono::seconds(60));
        fs::remove(get_user_ban_file(user_name, client_address));
    }).detach();
}

void BlackListHandler::increase_wrong_tries(const std::string &user_name, const std::string &client_address) {
    try {
        if (!fs::exists(BAN_DIRECTORY)) fs::create_directory(BAN_DIRECTORY);
        int bans_amount = get_bans_amount(user_name, client_address);

        bans_amount++;
        std::ofstream file_stream(get_user_ban_file(user_name, client_address));
        if (!file_stream.is_open()) throw InternalServerError("Internal Server Error: Failed to open ban file for increasing bans amount...");
        file_stream << bans_amount;
        file_stream.close();
    } catch (const std::exception &) {
        throw InternalServerError("Internal Error : failed to increase bans amount...");
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
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : failed to obtain bans amount...");
    }
}
