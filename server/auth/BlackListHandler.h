#pragma once

#include <string>

class BlackListHandler {
private:
    static const std::string BAN_DIRECTORY;

    static std::string get_user_file(const std::string &user_name, const std::string &client_address);

public:
    static int get_bans_amount(const std::string &user_name, const std::string &client_address);

    static bool is_banned(const std::string &user_name, const std::string &client_address);

    static void ban(const std::string &user_name, const std::string &client_address);

    static void increase_wrong_tries(const std::string &user_name, const std::string &client_address);

};
