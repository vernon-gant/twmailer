#include "BlackListHandler.h"
#include "errors/InternalServerError.h"
#include <sstream>
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
        fs::remove(get_user_file(user_name, client_address));
    }).detach();
}

void BlackListHandler::increase_wrong_tries(const std::string &user_name, const std::string &client_address) {
    try {
        int bans_amount = get_bans_amount(user_name, client_address);

        bans_amount++;
        std::ofstream file_stream(get_user_file(user_name, client_address));
        file_stream << bans_amount;
        file_stream.close();
    } catch (const std::exception &) {
        throw InternalServerError("Internal Error : failed to increase bans amount...");
    }
}

std::string BlackListHandler::get_user_file(const std::string &user_name, const std::string &client_address) {
    return BAN_DIRECTORY + "/" + user_name + "#" + client_address;
}

int BlackListHandler::get_bans_amount(const std::string &user_name, const std::string &client_address) {
    try {
        if (!fs::exists(get_user_file(user_name, client_address))) return 0;

        std::istringstream file_stream(get_user_file(user_name, client_address));
        std::string line;
        std::getline(file_stream, line);

        return std::stoi(line);
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : failed to obtain bans amount...");
    }
}
