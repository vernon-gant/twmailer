#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#define PORT 9999

typedef struct Mail {
    std::string sender, receiver, subject, message;
} Mail;

// Forward declarations of functions
std::string handleSendCommand();

std::string handleListCommand();

std::string handleReadCommand();

std::string handleDelCommand();

std::string formatMail(const Mail &mail);

Mail scanMail();

std::string getUserInput(const std::string &prompt);

void displayMenu();

void sendFormattedMessage(int socket, const std::string &message);

void receiveServerResponse(int socket);

std::string handleSendCommand() {
    std::cout << "\n[Send Email]\n";
    Mail mail = scanMail();
    return formatMail(mail);
}


std::string handleListCommand() {
    std::cout << "\n[Listing Emails]\n";
    return "LIST\n" + getUserInput("Enter Username: ") + "\n";
}

std::string handleReadCommand() {
    std::cout << "\n[Reading Email]\n";
    return "READ\n" + getUserInput("Enter Username: ") + "\n" +
           getUserInput("Enter Message Number: ") + "\n";
}

std::string handleDelCommand() {
    std::cout << "\n[Deleting Email]\n";
    return "DEL\n" + getUserInput("Enter Username: ") + "\n" +
           getUserInput("Enter Message Number: ") + "\n";
}

void displayMenu() {
    std::cout << "\n+-------------------------------------+\n";
    std::cout << "|             Command Menu            |\n";
    std::cout << "+-------------------------------------+\n";
    std::cout << "| SEND  - Send a new email            |\n";
    std::cout << "| LIST  - List emails                 |\n";
    std::cout << "| READ  - Read a specific email       |\n";
    std::cout << "| DEL   - Delete a specific email     |\n";
    std::cout << "| QUIT  - Exit the program            |\n";
    std::cout << "+-------------------------------------+\n\n";
}

void sendFormattedMessage(int socket, const std::string &message) {
    int messageLength = static_cast<int>(message.size());
    send(socket, reinterpret_cast<const char *>(&messageLength), sizeof(messageLength), 0);
    send(socket, message.c_str(), message.length(), 0);
}

void receiveServerResponse(int socket) {
    std::cout << "\nAwaiting _response from server...\n";
    int message_length;
    int bytes_received = recv(socket, &message_length, sizeof(message_length), 0);

    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            std::cout << "Server closed remote socket\n";
        } else {
            perror("recv error");
        }
        return;
    }

    // Allocate buffer based on the received length
    std::vector<char> buffer(message_length + 1, '\0');
    int total_bytes_received = 0;

    while (total_bytes_received < message_length) {
        bytes_received = recv(socket, buffer.data() + total_bytes_received, message_length - total_bytes_received, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "Server closed remote socket\n";
            } else {
                perror("recv error");
            }
            return;
        }
        total_bytes_received += bytes_received;
    }

    // Print the server _response
    std::cout << "\n|    Server _response received    |\n\n" << buffer.data() << std::endl;
}

std::string formatMail(const Mail &mail) {
    std::cout << "\nFormatting email for sending...\n";
    std::string mailStr =
            "SEND\n" + mail.sender + "\n" + mail.receiver + "\n" + mail.subject + "\n" + mail.message;
    return mailStr;
}

Mail scanMail() {
    Mail mail;
    std::cout << "Enter Sender: ";
    std::getline(std::cin, mail.sender);
    std::cout << "Enter Receiver: ";
    std::getline(std::cin, mail.receiver);
    std::cout << "Enter Subject: ";
    std::getline(std::cin, mail.subject);
    std::cout << "Enter Message (end with a line containing only '.'): ";
    std::string line;
    while (std::getline(std::cin, line) && line != ".") {
        mail.message += line + "\n";
    }
    return mail;
}

std::string getUserInput(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main(int argc, char **argv) {
    std::cout << "\nInitializing email client...\n\n";
    int create_socket;
    struct sockaddr_in address{};
    bool isQuit = false;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_aton(argc < 2 ? "127.0.0.1" : argv[1], &address.sin_addr);

    if (connect(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1) {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    printf("Connection with server (%s) established\n", inet_ntoa(address.sin_addr));

    while (!isQuit) {
        displayMenu();
        std::string command = getUserInput("Enter command: ");
        std::string message;

        if (command == "SEND") {
            message = handleSendCommand();
        } else if (command == "LIST") {
            message = handleListCommand();
        } else if (command == "READ") {
            message = handleReadCommand();
        } else if (command == "DEL") {
            message = handleDelCommand();
        } else if (command == "QUIT") {
            message = "QUIT\n";
            isQuit = true;
        } else {
            std::cout << "Invalid command." << std::endl;
            continue;
        }

        sendFormattedMessage(create_socket, message);
        if (!isQuit) receiveServerResponse(create_socket);
    }


    std::cout << "\nClosing connection and exiting program...\n";
    close(create_socket);

    return EXIT_SUCCESS;
}
