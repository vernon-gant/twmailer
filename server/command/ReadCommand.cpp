#include <filesystem>
#include <fstream>
#include "ReadCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "errors/InternalServerError.h"

namespace fs = std::filesystem;

void ReadCommand::execute() {
    try {
        std::string user_directory = _file_system_utils->get_user_directory(_user_context.user_name);
        std::string filename = user_directory + "/message_" + std::to_string(_message_number);

        std::ifstream file_stream(filename);
        if (!file_stream) throw InternalServerError("Internal error : could not open file");

        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        file_stream.close();

        _response = "OK\n" + buffer.str();
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal error : I/O problems...");
    }
}

void ReadCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_read(*this);
}

ReadCommand::ReadCommand(const UserContext &userContext, int message_number,
                         const std::shared_ptr<FileSystemUtils> &utils) : Command(
        userContext), _message_number(message_number), _file_system_utils(utils) {}

int ReadCommand::get_message_number() const {
    return _message_number;
}
