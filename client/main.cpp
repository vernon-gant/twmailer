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
    std::string receiver, subject, message;
} Mail;

// Forward declarations of functions
std::string handleLoginCommand(int socket);
std::string handleSendCommand();
std::string handleListCommand();
std::string handleReadCommand();
std::string handleDelCommand();
std::string formatMail(const Mail &mail);
Mail scanMail();
std::string getUserInput(const std::string &prompt);
void displayMenu();
void sendFormattedMessage(int socket, const std::string &message);
std::string receiveServerResponse(int socket);

std::string interpretServerResponse(const std::string& response) {
    if (response == "OK") {
        return "Authentication successful!";
    } else if (response == "WRONG") {
        return "Wrong credentials: wrong username or password...";
    } else if (response == "IS BANNED") {
        return "Your account is currently banned...";
    } else if (response == "GOT BANNED") {
        return "You have been banned after 3 login attempts. Try again in 1 minute...";
    } else {
        return response;
    }
}


std::string handleLoginCommand(int socket) {
    std::string username = getUserInput("Enter LDAP username: ");
    std::string password = getUserInput("Enter password: ");
    std::string loginMessage = "LOGIN\n" + username + "\n" + password + "\n";
    sendFormattedMessage(socket, loginMessage);
    return receiveServerResponse(socket);
}

std::string handleSendCommand() {
    std::cout << "\n[Send Email]\n";
    Mail mail = scanMail();
    return formatMail(mail);
}

std::string handleListCommand() {
    return "LIST\n";
}

std::string handleReadCommand() {
    std::string msgNum = getUserInput("Enter Message Number: ");
    return "READ\n" + msgNum + "\n";
}

std::string handleDelCommand() {
    std::string msgNum = getUserInput("Enter Message Number: ");
    return "DEL\n" + msgNum + "\n";
}

std::string formatMail(const Mail &mail) {
    std::string mailStr =
            "SEND\n" + mail.receiver + "\n" + mail.subject + "\n" + mail.message + ".\n";
    return mailStr;
}

Mail scanMail() {
    Mail mail;
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

std::string receiveServerResponse(int socket) {
    int message_length;
    int bytes_received = recv(socket, &message_length, sizeof(message_length), 0);

    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            std::cout << "Server closed remote socket\n";
        } else {
            perror("recv error");
        }
        return "Connection error or server closed connection";
    }

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
            return "Connection error or server closed connection";
        }
        total_bytes_received += bytes_received;
    }

    return std::string(buffer.data());
}

int main(int argc, char **argv) {
    std::cout << "\nInitializing email client...\n\n";
    int create_socket;
    struct sockaddr_in address{};
    bool isAuthenticated = false;
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

    while (!isAuthenticated && !isQuit) {
        std::string loginResult = handleLoginCommand(create_socket);
        if (loginResult == "OK") {
            isAuthenticated = true;
        } else {
            std::cout << interpretServerResponse(loginResult) << std::endl;
            if (loginResult == "IS BANNED" || loginResult == "GOT BANNED") break;
            isQuit = getUserInput("Try again? (yes/no): ") != "yes";
        }
    }

    while (!isQuit && isAuthenticated) {
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
        if (!isQuit) {
            std::string response = receiveServerResponse(create_socket);
            std::cout << "\nServer response: " << response << std::endl;
        }
    }

    std::cout << "\nClosing connection and exiting program...\n";
    close(create_socket);

    return EXIT_SUCCESS;
}
