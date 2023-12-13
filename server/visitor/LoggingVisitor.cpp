#include <iostream>
#include "LoggingVisitor.h"

void LoggingVisitor::visit_list(const ListCommand &list_command) const {
    const UserContext &user_context = list_command.get_user_context();
    std::cout << user_context.client_ip_address << "  #############  LIST command received - User: "
              << user_context.user_name
              << std::endl;
}

void LoggingVisitor::visit_send(const SendCommand &send_command) const {
    const Mail &mail = send_command.get_mail();
    const UserContext &user_context = send_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  SEND command received - Sender: " << mail.sender
              << ", Receiver: " << mail.receiver
              << ", Subject: " << mail.subject
              << std::endl;
}

void LoggingVisitor::visit_read(const ReadCommand &read_command) const {
    const UserContext &user_context = read_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  READ command received - User: " << user_context.user_name
              << ", Message Number: " << read_command.get_message_number()
              << std::endl;
}

void LoggingVisitor::visit_delete(const DeleteCommand &delete_command) const {
    const UserContext &user_context = delete_command.get_user_context();
    std::cout << user_context.client_ip_address
              << "  #############  DELETE command received - User: " << user_context.user_name
              << ", Message Number: " << delete_command.get_message_number()
              << std::endl;
}

void LoggingVisitor::visit_quit(const QuitCommand &quit_command) const {
    const UserContext &user_context = quit_command.get_user_context();
    std::cout << user_context.client_ip_address << "  #############  QUIT command received from User: "
              << user_context.user_name
              << std::endl;
}
