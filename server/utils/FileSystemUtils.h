#pragma once

#include <filesystem>
#include <string>
#include "Environment.h"

namespace fs = std::filesystem;

class FileSystemUtils {
private:
    std::shared_ptr<Environment> environment;
public:
    explicit FileSystemUtils(const std::shared_ptr<Environment> &environment) : environment(environment) {}

    static int count_files(const std::string &user_directory) {
        if (!fs::exists(user_directory)) throw std::runtime_error("No such user");
        int count = 0;
        for ([[maybe_unused]] const auto &entry: fs::directory_iterator(user_directory)) count++;
        return count;
    }

    std::string get_user_directory(const std::string &user_name) {
        return environment->MAIL_DIRECTORY() + "/" + user_name;
    }
};
