#pragma once

#include <iostream>
#include <string>

class Logger
{
public:
    static void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    static void logError(const std::string &errorMessage)
    {
        std::cerr << "\033[31m [ERROR] " << errorMessage << "\033[0m" << "\n";
    }

    static void logWarning(const std::string &warningMessage)
    {
        std::cerr << "\033[33m [WARNING] " << warningMessage << "\033[0m" << "\n";
    }

    static void logInfo(const std::string &infoMessage)
    {
        std::cout << "\033[32m [INFO] " << infoMessage << "\033[0m" << "\n";
    }
};