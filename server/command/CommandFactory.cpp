#include <stdexcept>
#include <limits>
#include "CommandFactory.h"
#include "errors/ValidationError.h"

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
    mail.sender = user_context.user_name;

    std::istringstream iss(message);
    std::getline(iss, mail.receiver);
    std::getline(iss, mail.subject);

    std::vector<std::string> contentLines;
    std::string line;
    while (std::getline(iss, line)) contentLines.push_back(line);
    mail.content = join(contentLines, "\n");
    return std::make_unique<SendCommand>(user_context, mail, _file_system_utils);
}

std::unique_ptr<Command>
CommandFactory::create_list_command(const std::string &, const UserContext &user_context) {
    return std::make_unique<ListCommand>(user_context, _file_system_utils);
}

std::unique_ptr<Command>
CommandFactory::create_read_command(const std::string &message, const UserContext &user_context) {
    int message_number = parse_message_number(message);
    return std::make_unique<ReadCommand>(user_context, message_number, _file_system_utils);
}

std::unique_ptr<Command>
CommandFactory::create_del_command(const std::string &message, const UserContext &user_context) {
    int message_number = parse_message_number(message);
    return std::make_unique<DeleteCommand>(user_context, message_number, _file_system_utils);
}

std::unique_ptr<Command>
CommandFactory::create_quit_command(const std::string &, const UserContext &user_context) {
    return std::make_unique<QuitCommand>(user_context);
}

int CommandFactory::parse_message_number(const std::string &message) {
    std::istringstream iss(message);
    int message_number;

    std::string message_number_as_string;
    std::getline(iss, message_number_as_string);
    try {
        message_number = message_number_as_string.empty() ? std::numeric_limits<int>::max() : std::stoi(
                message_number_as_string);
    } catch (const std::invalid_argument &exception) {
        throw ValidationError("Validation error : invalid message number format - must be integer!");
    }

    return message_number;
}

CommandFactory::CommandFactory(const std::shared_ptr<FileSystemUtils> &fileSystemUtils)
        : _file_system_utils(fileSystemUtils),
          COMMAND_MAP({
                              {"SEND", [this](const std::string &message, const UserContext &user_context) {
                                  return this->create_send_command(message, user_context);
                              }},
                              {"LIST", [this](const std::string &message, const UserContext &user_context) {
                                  return this->create_list_command(message, user_context);
                              }},
                              {"READ", [this](const std::string &message, const UserContext &user_context) {
                                  return this->create_read_command(message, user_context);
                              }},
                              {"DEL",  [this](const std::string &message, const UserContext &user_context) {
                                  return this->create_del_command(message, user_context);
                              }},
                              {"QUIT", [this](const std::string &message, const UserContext &user_context) {
                                  return this->create_quit_command(message, user_context);
                              }}
                      }) {}

