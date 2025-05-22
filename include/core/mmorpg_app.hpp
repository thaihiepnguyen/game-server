#pragma once

#include <string>
#include "core/command/command.hpp"
#include "core/protocol/protocol.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <cxxabi.h>
#include "core/database/db_connection.hpp"

class MMORPGApplication {
private:
    short _port;
    std::shared_ptr<ICommand> _authMiddleware;
    std::unordered_map<std::shared_ptr<ICommand>, std::shared_ptr<ICommand>> _authMap;
    std::vector<std::shared_ptr<ICommand>> _middlewares;
    std::unordered_map<Protocol::Command, std::shared_ptr<ICommand>> _commands;
    std::shared_ptr<IDatabaseConnection> _dbConnection;

    std::map<std::string, Protocol::Value> _handleCommand(Protocol::Command id, const std::map<std::string, Protocol::Value>& request);
    /**
     * Convert a mangled C++ type name to a human-readable format.
     * 
     * @brief Demangle a C++ type name
     * @param name The mangled type name
     * @return The demangled type name
     */
    std::string _demangle(const char* name) {
        int status = -1;
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result = (status == 0 && demangled != nullptr) ? demangled : name;
        std::free(demangled);
        return result;
    }
public:
    MMORPGApplication(short port) : _port(port) {}

    MMORPGApplication* registerAuthMiddleware(ICommand* middleware) {
        _authMiddleware = std::shared_ptr<ICommand>(middleware);
        return this;
    }

    MMORPGApplication* registerMiddleware(ICommand* middleware) {
        _middlewares.push_back(std::shared_ptr<ICommand>(middleware));
        return this;
    }

    MMORPGApplication* registerCommand(Protocol::Command id, ICommand* command, bool isPublic = false) {
        if (!isPublic) {
            _authMap[std::shared_ptr<ICommand>(command)] = std::shared_ptr<ICommand>(command);
        }
        _commands[id] = std::shared_ptr<ICommand>(command);

        std::cout << "\033[32m" << "Command registered: " 
            << _demangle(typeid(*command).name()) 
            << " -> command " << static_cast<unsigned int>(id) << "\033[0m" << "\n";
        return this;
    }

    MMORPGApplication* registerDatabaseConnection(IDatabaseConnection* dbConnection) {
        _dbConnection = std::shared_ptr<IDatabaseConnection>(dbConnection);
        return this;
    }
};