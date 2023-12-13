#include "DeleteCommand.h"

#include <utility>
#include "../visitor/CommandVisitor.h"
#include "exceptions/InternalServerError.h"

void DeleteCommand::execute() {
    const std::string &user_directory = _file_system_utils->get_user_directory(_user_context.user_name);
    const std::string &file_path = user_directory + "/message_" + std::to_string(_message_number);
    try {
        if (std::filesystem::remove(file_path)) _response = "OK\n";
        else _response = "ERR\n";
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : I/O exception");
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
