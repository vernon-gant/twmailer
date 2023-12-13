#include <regex>
#include <filesystem>
#include "ValidationVisitor.h"
#include "utils/Environment.h"
#include "errors/ValidationError.h"
#include "utils/FileSystemUtils.h"

namespace fs = std::filesystem;

void ValidationVisitor::visit_list(const ListCommand &list_command) const {
    std::string user_directory = _file_system_utils->get_user_directory(list_command.get_user_name());

    if (!fs::exists(user_directory)) throw ValidationError("0\n");
}

void ValidationVisitor::visit_send(const SendCommand &send_command) const {
    const Mail &mail = send_command.get_mail();

    std::regex username_regex("^[a-zA-Z0-9]{1,8}$");

    if (!std::regex_match(mail.sender, username_regex)) {
        throw ValidationError("Validation error: invalid sender username: " + mail.sender +
                              " -> must consist of numbers with letters + length <= 8)...");
    }

    if (mail.receiver.empty()) throw ValidationError("Invalid SEND command format: missing receiver...");
    if (!std::regex_match(mail.receiver, username_regex)) {
        throw ValidationError("Validation error: invalid receiver username: " + mail.receiver +
                              +" -> must consist of numbers with letters + length <= 8)");
    }

    if (mail.sender == mail.receiver) throw ValidationError("Validation error: sender and receiver must be different...");
    if (mail.subject.empty()) throw ValidationError("Invalid SEND command format: missing subject...");
    if (mail.subject.size() > 80) throw ValidationError("Validation error: subject too long (max 80 characters)...");
}


void ValidationVisitor::visit_read(const ReadCommand &read_command) const {
    validate_single_message_command("READ", read_command.get_message_number(), read_command.get_user_context().user_name);
}

void ValidationVisitor::visit_delete(const DeleteCommand &delete_command) const {
    validate_single_message_command("DEL", delete_command.get_message_number(), delete_command.get_user_context().user_name);
}

void ValidationVisitor::visit_quit(const QuitCommand &) const {}

void ValidationVisitor::validate_single_message_command(const std::string &command_name, int message_number,
                                                        const std::string &user_name) const {
    if (message_number == std::numeric_limits<int>::max())
        throw ValidationError("Invalid " + command_name + " command format: message number is not specified...");

    std::string user_directory = _file_system_utils->get_user_directory(user_name);

    if (!fs::exists(user_directory)) throw ValidationError("Validation error : there are no messages in your inbox yet...");

    std::string message_file = user_directory + "/message_" + std::to_string(message_number);

    if (!fs::exists(message_file)) throw ValidationError("Validation error: specified message does not exist...");
}

ValidationVisitor::ValidationVisitor(const std::shared_ptr<FileSystemUtils> &fileSystemUtils) : _file_system_utils(
        fileSystemUtils) {}
