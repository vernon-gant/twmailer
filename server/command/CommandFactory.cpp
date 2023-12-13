#include <stdexcept>
#include <limits>
#include "CommandFactory.h"
#include "exceptions/ValidationError.h"

const std::unordered_map<std::string, CommandFactory::CommandCreator> CommandFactory::COMMAND_MAP = {
        {"SEND", &CommandFactory::create_send_command},
        {"LIST", &CommandFactory::create_list_command},
        {"READ", &CommandFactory::create_read_command},
        {"DEL",  &CommandFactory::create_del_command},
        {"QUIT", &CommandFactory::create_quit_command}
};

std::unique_ptr<Command> CommandFactory::create(std::string &message, const UserContext &user_context) {
    if (message.empty()) throw std::invalid_argument("Empty command string!");

    size_t end = message.find('\n');
    if (end == std::string::npos) throw std::invalid_argument("Command not properly terminated with newline!");

    std::string commandName = message.substr(0, end);
    if (commandName.empty()) throw std::invalid_argument("Empty command name!");
    message = message.substr(end + 1);

    auto iterator = COMMAND_MAP.find(commandName);
    if (iterator != COMMAND_MAP.end()) return iterator->second(message, user_context);

    throw std::runtime_error("Unknown command: " + commandName + "!");
}

std::string CommandFactory::join(const std::vector<std::string> &strings, const std::string &delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) oss << delimiter;
        oss << strings[i];
    }
    return oss.str();
}

std::unique_ptr<Command>
CommandFactory::create_send_command(const std::string &message, const UserContext &user_context) {
    Mail mail;

    std::istringstream iss(message);
    std::getline(iss, mail.sender);
    std::getline(iss, mail.receiver);
    std::getline(iss, mail.subject);

    std::vector<std::string> contentLines;
    std::string line;
    while (std::getline(iss, line)) contentLines.push_back(line);
    mail.content = join(contentLines, "\n");
    return std::make_unique<SendCommand>(user_context, mail);
}

std::unique_ptr<Command>
CommandFactory::create_list_command(const std::string &message, const UserContext &user_context) {
    std::istringstream iss(message);
    std::string user_name;
    std::getline(iss, user_name);
    UserContext new_user_context = {.client_ip_address = user_context.client_ip_address, .user_name = user_name};
    return std::make_unique<ListCommand>(new_user_context);
}

std::unique_ptr<Command>
CommandFactory::create_read_command(const std::string &message, const UserContext &user_context) {
    return std::make_unique<ReadCommand>(user_context, create_single_message_request(message));
}

std::unique_ptr<Command>
CommandFactory::create_del_command(const std::string &message, const UserContext &user_context) {
    return std::make_unique<DeleteCommand>(user_context, create_single_message_request(message));
}

std::unique_ptr<Command>
CommandFactory::create_quit_command(const std::string &, const UserContext &user_context) {
    return std::make_unique<QuitCommand>(user_context);
}

SingleMessageRequest CommandFactory::create_single_message_request(const std::string &message) {
    std::istringstream iss(message);
    SingleMessageRequest message_request;

    std::getline(iss, message_request.user_name);

    std::string message_number;
    std::getline(iss, message_number);
    try {
        message_request.message_number = message_number.empty() ? std::numeric_limits<int>::max() : std::stoi(
                message_number);
    } catch (const std::invalid_argument &exception) {
        throw ValidationError("Validation error : invalid message number format - must be integer!");
    }

    return message_request;
}
