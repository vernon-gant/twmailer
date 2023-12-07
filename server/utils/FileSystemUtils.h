#include <filesystem>
#include <string>
#include "Environment.h"

namespace fs = std::filesystem;

class FileSystemUtils {
public:
    static int countFilesInDirectory(const std::string &user_directory) {
        if (!fs::exists(user_directory)) throw std::runtime_error("No such user");
        int count = 0;
        for ([[maybe_unused]] const auto &entry: fs::directory_iterator(user_directory)) count++;
        return count;
    }

    static std::string get_user_directory(const std::string &user_name) {
        return Environment::MAIL_DIRECTORY() + "/" + user_name;
    }
};
