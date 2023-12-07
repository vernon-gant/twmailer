#include <regex>
#include <filesystem>
#include "ValidationVisitor.h"
#include "utils/Environment.h"
#include "exceptions/ValidationError.h"
#include "utils/FileSystemUtils.h"

namespace fs = std::filesystem;

void ValidationVisitor::visitList(const ListCommand &list_command) const {
    const std::string &user_name = list_command.get_user_name();

    if (user_name.empty()) throw ValidationError("Invalid LIST command format: missing username!");

    std::string user_directory = FileSystemUtils::get_user_directory(user_name);

    if (!fs::exists(user_directory)) throw ValidationError("0\n");
}

void ValidationVisitor::visitSend(const SendCommand &send_command) const {
    const Mail &mail = send_command.get_mail();

    std::regex username_regex("^[a-zA-Z0-9]{1,8}$");

    if (mail.sender.empty()) throw ValidationError("Invalid SEND command format: missing sender!");
    if (!std::regex_match(mail.sender, username_regex)) {
        throw ValidationError("Validation error: invalid sender username: " + mail.sender +
                              " -> must consist of numbers with letters + length <= 8)");
    }

    if (mail.receiver.empty()) throw ValidationError("Invalid SEND command format: missing receiver!");
    if (!std::regex_match(mail.receiver, username_regex)) {
        throw ValidationError("Validation error: invalid receiver username: " + mail.receiver +
                              +" -> must consist of numbers with letters + length <= 8)");
    }

    if (mail.sender == mail.receiver) throw ValidationError("Validation error: sender and receiver must be different!");
    if (mail.subject.empty()) throw ValidationError("Invalid SEND command format: missing subject!");
    if (mail.subject.size() > 80) throw ValidationError("Validation error: subject too long (max 80 characters)");
}


void ValidationVisitor::visitRead(const ReadCommand &read_command) const {
    validateSingleMessageCommand(read_command.get_single_message_request(), "READ");
}

void ValidationVisitor::visitDelete(const DeleteCommand &delete_command) const {
    validateSingleMessageCommand(delete_command.get_single_message_request(), "DEL");
}

void ValidationVisitor::visitQuit(const QuitCommand &) const {}

void ValidationVisitor::validateSingleMessageCommand(const SingleMessageRequest &message_request,
                                                     const std::string &command_name) {
    if (message_request.user_name.empty())
        throw ValidationError("Invalid " + command_name + " command format: missing user name.");
    if (message_request.message_number == std::numeric_limits<int>::max())
        throw ValidationError("Invalid " + command_name + " command format: message number is not specified.");

    std::string user_directory = FileSystemUtils::get_user_directory(message_request.user_name);

    if (!fs::exists(user_directory)) throw ValidationError("Validation error: user directory does not exist.");

    std::string message_file = user_directory + "/message_" + std::to_string(message_request.message_number);

    if (!fs::exists(message_file)) throw ValidationError("Validation error: specified message does not exist.");
}
