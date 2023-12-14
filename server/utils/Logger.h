#pragma once

#include <string>
#include <iostream>

class Logger {
public:
    void log(const std::string &client_address, const std::string &message) {
        std::cout << client_address << "  #############  " << message << std::endl;
    }

    void log(const std::string &message) {
        std::cout << message << std::endl;
    }

    void log_fatal_error(const std::string &error) {
        std::cout << "\n               FATAL ERROR               \n\n" << error << std::endl;
    }

};
