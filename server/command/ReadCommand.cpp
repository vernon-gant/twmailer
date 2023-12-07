#include <filesystem>
#include <fstream>
#include "ReadCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "exceptions/InternalServerError.h"

namespace fs = std::filesystem;

void ReadCommand::execute() {
    try {
        std::string base_path = Environment::MAIL_DIRECTORY();
        std::string user_dir = base_path + "/" + _message_request.user_name;
        std::string filename = user_dir + "/message_" + std::to_string(_message_request.message_number);

        std::ifstream file_stream(filename);
        if (!file_stream) throw InternalServerError("Internal error : could not open file");

        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        file_stream.close();

        response = "OK\n" + buffer.str();
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal error : I/O problems...");
    }
}

void ReadCommand::accept(const CommandVisitor &visitor) const {
    visitor.visitRead(*this);
}

const SingleMessageRequest &ReadCommand::get_single_message_request() const {
    return _message_request;
}

ReadCommand::ReadCommand(const UserContext &userContext, const SingleMessageRequest &messageRequest) : Command(
        userContext), _message_request(messageRequest) {}
