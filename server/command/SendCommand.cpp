#include "SendCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "errors/InternalServerError.h"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <utility>

namespace fs = std::filesystem;

std::mutex SendCommand::file_mutex;

void SendCommand::execute() {
    try {
        std::lock_guard<std::mutex> lock(file_mutex);
        std::string user_directory = _file_system_utils->get_user_directory(_mail.sender);

        if (!fs::exists(user_directory) && !fs::create_directory(user_directory))
            throw std::runtime_error("Failed to create directory: " + user_directory);

        int messages_count = _file_system_utils->count_files(user_directory);
        std::string filename = user_directory + "/message_" + std::to_string(messages_count + 1);

        std::ofstream fileStream(filename);
        if (!fileStream) throw std::runtime_error("Unable to open file for writing: " + filename);

        fileStream << "Subject: " << _mail.subject << "\n";
        fileStream << "Receiver: " << _mail.receiver << "\n";
        fileStream << "Content:\n" << _mail.content;
        fileStream.close();
        _response = "OK";
    } catch (const std::ofstream::failure &exception) {
        throw InternalServerError("Internal Server error : could not save sent message...");
    }
}

void SendCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_send(*this);
}

const Mail &SendCommand::get_mail() const {
    return _mail;
}

SendCommand::SendCommand(const UserContext &userContext, Mail mail, const std::shared_ptr<FileSystemUtils> &utils)
        : Command(userContext), _mail(std::move(mail)), _file_system_utils(utils) {}
