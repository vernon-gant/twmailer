#include "SendCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "utils/FileSystemUtils.h"
#include "exceptions/InternalServerError.h"
#include <filesystem>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

void SendCommand::execute() {
    try {
        std::string user_directory = FileSystemUtils::get_user_directory(_mail.sender);

        if (!fs::exists(user_directory) && !fs::create_directory(user_directory))
            throw std::runtime_error("Failed to create directory: " + user_directory);

        int messages_count = FileSystemUtils::countFilesInDirectory(user_directory);
        std::string filename = user_directory + "/message_" + std::to_string(messages_count + 1);

        std::ofstream fileStream(filename);
        if (!fileStream) throw std::runtime_error("Unable to open file for writing: " + filename);

        fileStream << "Subject: " << _mail.subject << "\n";
        fileStream << "Receiver: " << _mail.receiver << "\n";
        fileStream << "Content:\n" << _mail.content << "\n";
        fileStream.close();
        response = "OK";
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : I/O exception");
    }
}

void SendCommand::accept(const CommandVisitor &visitor) const {
    visitor.visitSend(*this);
}

const Mail &SendCommand::get_mail() const {
    return _mail;
}

SendCommand::SendCommand(const UserContext &userContext, const Mail &mail) : Command(userContext), _mail(mail) {}
