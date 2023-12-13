#include <filesystem>
#include <fstream>
#include "ListCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "exceptions/InternalServerError.h"

namespace fs = std::filesystem;

void ListCommand::execute() {
    std::string user_directory = _file_system_utils->get_user_directory(_user_context.user_name);
    std::string subjects;
    int message_count = 0;

    try {
        for (const auto &entry: fs::directory_iterator(user_directory)) {
            std::ifstream file_stream(entry.path().string());
            if (!file_stream.is_open())
                throw InternalServerError("Internal error : could not open file " + entry.path().string());
            std::string subject;
            std::getline(file_stream, subject);
            std::getline(file_stream, subject);
            subjects.append(subject + "\n");
            message_count++;
        }
        _response = std::to_string(message_count) + "\n" + subjects;
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : I/O exception");
    }
}

void ListCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_list(*this);
}

const std::string &ListCommand::get_user_name() const {
    return _user_context.user_name;
}

ListCommand::ListCommand(const UserContext &userContext, const std::shared_ptr<FileSystemUtils> &utils) : Command(
        userContext), _file_system_utils(utils) {}
