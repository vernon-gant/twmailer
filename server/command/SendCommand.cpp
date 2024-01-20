#include "SendCommand.h"
#include "../visitor/CommandVisitor.h"
#include "utils/Environment.h"
#include "exceptions/InternalServerError.h"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <mutex>
#include <utility>

namespace fs = std::filesystem;

std::mutex SendCommand::send_mutex; // Define the static mutex

void SendCommand::execute() {
    std::lock_guard<std::mutex> guard(send_mutex);  // Locks the mutex here

    try {
        std::string user_directory = _file_system_utils->get_user_directory(_mail.sender);

        if (!fs::exists(user_directory) && !fs::create_directory(user_directory))
            throw std::runtime_error("Failed to create directory: " + user_directory);

        int messages_count = _file_system_utils->count_files(user_directory);
        std::string filename = user_directory + "/message_" + std::to_string(messages_count + 1);

        std::ofstream fileStream(filename);
        if (!fileStream) throw std::runtime_error("Unable to open file for writing: " + filename);

        fileStream << "Subject: " << _mail.subject << "\n";
        fileStream << "Receiver: " << _mail.receiver << "\n";
        fileStream << "Content:\n" << _mail.content << "\n";
        fileStream.close();
        _response = "OK";
    } catch (const std::exception &exception) {
        throw InternalServerError("Internal Server error : I/O exception");
    }

    // The mutex is automatically released when the lock_guard goes out of scope
}

void SendCommand::accept(const CommandVisitor &visitor) const {
    visitor.visit_send(*this);
}

const Mail &SendCommand::get_mail() const {
    return _mail;
}

SendCommand::SendCommand(const UserContext &userContext, Mail mail, const std::shared_ptr<FileSystemUtils>& utils)
        : Command(userContext), _mail(std::move(mail)), _file_system_utils(utils) {}
