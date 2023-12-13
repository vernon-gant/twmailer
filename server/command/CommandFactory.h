#pragma once

#include <memory>
#include <functional>
#include <sstream>
#include "Command.h"
#include "SendCommand.h"
#include "ListCommand.h"
#include "ReadCommand.h"
#include "DeleteCommand.h"
#include "QuitCommand.h"

typedef struct SingleMessageRequest {
    std::string user_name;
    int message_number;
} SingleMessageRequest;


class CommandFactory {
private:
    std::shared_ptr<FileSystemUtils> _file_system_utils;


    using CommandCreator = std::function<std::unique_ptr<Command>(const std::string &, const UserContext &)>;

    const std::unordered_map<std::string, CommandCreator> COMMAND_MAP;

    static std::string join(const std::vector<std::string> &strings, const std::string &delimiter);

    std::unique_ptr<Command> create_send_command(const std::string &message, const UserContext &user_context);

    std::unique_ptr<Command>
    create_list_command(const std::string &message, const UserContext &user_context);

    std::unique_ptr<Command>
    create_read_command(const std::string &message, const UserContext &user_context);

    std::unique_ptr<Command>
    create_del_command(const std::string &message, const UserContext &user_context);

    std::unique_ptr<Command>
    create_quit_command(const std::string &message, const UserContext &user_context);

    static SingleMessageRequest create_single_message_request(const std::string &message);

public:
    explicit CommandFactory(const std::shared_ptr<FileSystemUtils> &fileSystemUtils);

    std::unique_ptr<Command> create(std::string &message, const UserContext &user_context);
};
