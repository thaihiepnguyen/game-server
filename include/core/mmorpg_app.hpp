#pragma once

#include <string>
#include "core/command/command.hpp"
#include "core/protocol/protocol.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <cxxabi.h>
#include "core/database/db_connection.hpp"
#include <functional>
#include "network/tcp_server.hpp"
#include "core/repository/repository_register.hpp"

class MMORPGApplication {
private:
    short _port;
    std::shared_ptr<ICommand> _authMiddleware;

    std::unordered_map<std::shared_ptr<ICommand>, std::shared_ptr<ICommand>> _authMap;
    std::vector<std::shared_ptr<ICommand>> _middlewares;
    std::unordered_map<Protocol::Command, std::shared_ptr<ICommand>> _commands;
    std::shared_ptr<IDatabaseConnection> _dbConnection;
    std::unordered_map<std::string, Protocol::Value> _handleCommand(Protocol::Command id, const std::unordered_map<std::string, Protocol::Value>& request);
    std::shared_ptr<RepositoryRegister> _repositoryRegister;
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
    MMORPGApplication() {
        _repositoryRegister = std::make_shared<RepositoryRegister>();
    }

    MMORPGApplication* registerAuthMiddleware(ICommand* middleware) {
        _authMiddleware = std::shared_ptr<ICommand>(middleware);

        std::cout << "\033[32m" << "Authentication Middleware registered: " 
            << _demangle(typeid(*middleware).name()) << "\033[0m" << "\n";
        return this;
    }

    MMORPGApplication* registerMiddleware(ICommand* middleware) {
        _middlewares.push_back(std::shared_ptr<ICommand>(middleware));

        std::cout << "\033[32m" << "Middleware registered: " 
            << _demangle(typeid(*middleware).name()) << "\033[0m" << "\n";
        return this;
    }

    MMORPGApplication* registerCommand(Protocol::Command id, ICommand* command, bool isPublic = false) {
        std::shared_ptr<ICommand> cmdPtr(command);

        if (!isPublic) {
            _authMap[cmdPtr] = _authMiddleware;
        }
        
        _commands[id] = cmdPtr;
        _commands[id]->inject(_repositoryRegister);
        std::cout << "\033[32m" << "Command registered: " 
            << _demangle(typeid(*command).name()) 
            << " -> command " << static_cast<unsigned int>(id) << "\033[0m" << "\n";
        return this;
    }

    MMORPGApplication* registerRepository(IRepository* repository) {
        _repositoryRegister->registerRepository(std::shared_ptr<IRepository>(repository));
        std::cout << "\033[32m" << "Repository registered: " 
            << _demangle(typeid(*repository).name()) << "\033[0m" << "\n";
        return this;
    }

    MMORPGApplication* registerDatabaseConnection(IDatabaseConnection* dbConnection) {
        _dbConnection = std::shared_ptr<IDatabaseConnection>(dbConnection);
        return this;
    }

    MMORPGApplication* listen(unsigned short port) {
        _port = port;
        auto server = std::make_shared<TCPServer>();
        server->run(_port, [this](const Protocol::Packet& packet) {
            return this->_handleCommand(packet.command, packet.data);
        });
        return this;
    }

    ~MMORPGApplication() {
        std::cout << "\033[31m" << "MMORPGApplication destructor called\n" << "\033[0m";
        if (_dbConnection) {
            _dbConnection->disconnect();
        }
    }
};