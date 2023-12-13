#include "DeleteCommand.h"

#include <utility>
#include "../visitor/CommandVisitor.h"
#include "exceptions/InternalServerError.h"

void DeleteCommand::execute() {
    const std::string &user_directory = _file_system_utils->get_user_directory(_message_request.user_name);
    const std::string &file_path = user_directory + "/message_" + std::to_string(_message_request.message_number);
    try {
        if (std::filesystem::remove(file_path)) _response = "OK\n";
        else _response = "ERR\n";
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : I/O exception");
    }
}

void DeleteCommand::accept(const CommandVisitor &visitor) const {
    visitor.visitDelete(*this);
}

const SingleMessageRequest &DeleteCommand::get_single_message_request() const {
    return _message_request;
}

DeleteCommand::DeleteCommand(const UserContext &userContext, SingleMessageRequest messageRequest, const std::shared_ptr<FileSystemUtils>& utils) : Command(
        userContext), _message_request(std::move(messageRequest)), _file_system_utils(utils) {}
