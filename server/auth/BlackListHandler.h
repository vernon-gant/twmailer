#pragma once

#include <string>
#include <mutex>

class BlackListHandler {
private:
    static const std::string BAN_DIRECTORY;

    static std::string get_user_ban_file(const std::string &user_name, const std::string &client_address);

    static std::mutex file_mutex;

public:
    static int get_bans_amount(const std::string &user_name, const std::string &client_address);

    static bool is_banned(const std::string &user_name, const std::string &client_address);

    static void ban(const std::string &user_name, const std::string &client_address);

    static void increase_wrong_tries(const std::string &user_name, const std::string &client_address);

    static void check_old_bans();

};
