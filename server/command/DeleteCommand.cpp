#include "DeleteCommand.h"

#include "../visitor/CommandVisitor.h"
#include "errors/InternalServerError.h"

std::mutex DeleteCommand::file_mutex;

void DeleteCommand::execute() {
    std::lock_guard<std::mutex> lock(file_mutex);
    const std::string &user_directory = _file_system_utils->get_user_directory(_user_context.user_name);
    const std::string &file_path = user_directory + "/message_" + std::to_string(_message_number);
    try {
        if (std::filesystem::remove(file_path)) {
            reorder_message_files(user_directory, _message_number);
            _response = "OK";
        } else _response = "ERR";
    } catch (const fs::filesystem_error &exception) {
        throw InternalServerError("Internal Server error : error while deleting the message...");
    }
}

void DeleteCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_delete(*this);
}

DeleteCommand::DeleteCommand(const UserContext &userContext, int message_number,
                             const std::shared_ptr<FileSystemUtils> &utils) : Command(
        userContext), _message_number(message_number), _file_system_utils(utils) {}

int DeleteCommand::get_message_number() const {
    return _message_number;
}

void DeleteCommand::reorder_message_files(const std::string &user_directory, int deleted_message_number) {
    for (int i = deleted_message_number + 1;; ++i) {
        std::string old_file = user_directory + "/message_" + std::to_string(i);
        if (!std::filesystem::exists(old_file)) break;
        std::string new_file = user_directory + "/message_" + std::to_string(i - 1);
        std::filesystem::rename(old_file, new_file);
    }
}

