#include <iostream>
#include "LoggingVisitor.h"

void LoggingVisitor::visitList(const ListCommand &list_command) const {
    const auto &user_context = list_command.get_user_context();
    std::cout << user_context.client_ip_address << "  #############  LIST command received - User: "
              << user_context.user_name
              << std::endl;
}

void LoggingVisitor::visitSend(const SendCommand &send_command) const {
    const auto &mail = send_command.get_mail();
    const auto &user_context = send_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  SEND command received - Sender: " << mail.sender
              << ", Receiver: " << mail.receiver
              << ", Subject: " << mail.subject
              << std::endl;
}

void LoggingVisitor::visitRead(const ReadCommand &read_command) const {
    const auto &message_request = read_command.get_single_message_request();
    const auto &user_context = read_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  READ command received - User: " << user_context.user_name
              << ", Message Number: " << message_request.message_number
              << std::endl;
}

void LoggingVisitor::visitDelete(const DeleteCommand &delete_command) const {
    const auto &message_request = delete_command.get_single_message_request();
    const auto &user_context = delete_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  DELETE command received - User: " << user_context.user_name
              << ", Message Number: " << message_request.message_number
              << std::endl;
}

void LoggingVisitor::visitQuit(const QuitCommand &quit_command) const {
    const auto &user_context = quit_command.get_user_context();
    std::cout << user_context.client_ip_address << "  #############  QUIT command received from User: "
              << user_context.user_name
              << std::endl;
}
