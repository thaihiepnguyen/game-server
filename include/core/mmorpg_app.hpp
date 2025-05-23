#pragma once

#include <string>
#include "core/command/command.hpp"
#include "core/protocol/protocol.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include "core/database/db_connection.hpp"
#include <functional>
#include "network/tcp_server.hpp"
#include "core/repository/repository_register.hpp"
#include "core/service/provider.hpp"
#include "core/utils/log.hpp"
#include "core/utils/string.hpp"

class MMORPGApplication {
private:
    short _port;
    std::shared_ptr<RepositoryRegister> _repositoryRegister;
    std::shared_ptr<Provider> _provider;

    std::shared_ptr<ICommand> _authMiddleware;
    std::unordered_map<std::shared_ptr<ICommand>, std::shared_ptr<ICommand>> _authMap;
    std::vector<std::shared_ptr<ICommand>> _middlewares;
    std::unordered_map<Protocol::Command, std::shared_ptr<ICommand>> _commands;

    std::shared_ptr<IDatabaseConnection> _dbConnection;
    std::unordered_map<std::string, Protocol::Value> _handleCommand(Protocol::Command id, const std::unordered_map<std::string, Protocol::Value>& request);


public:
    MMORPGApplication();

    /**
     * @brief This method registers the authentication middleware based on the command is public or not.
     * @param command The command object.
     * @note This method will be called first when a request is received.
     */
    MMORPGApplication* registerAuthMiddleware(ICommand* middleware);

    /**
     * @brief This method registers a middleware for the application.
     * @param middleware The middleware object.
     * @note This method will be called after the authentication middleware.
     */
    MMORPGApplication* registerMiddleware(ICommand* middleware);

    /**
     * @brief This method registers a command for the application.
     * @param id The command ID.
     * @param command The command object.
     * @param isPublic Whether the command is public or not for authentication.
     * @note This method will be called after the middlewares
     */
    MMORPGApplication* registerCommand(Protocol::Command id, ICommand* command, bool isPublic);

    /**
     * @brief This method registers a repository for the application to query the database.
     * @param repository The repository object.
     */
    MMORPGApplication* registerRepository(IRepository* repository);

    /**
     * @brief This method registers a service for the application.
     * @param service The service object.
     */
    MMORPGApplication* registerService(IService* service);

    /**
     * @brief This method registers a database connection for the application.
     * @param dbConnection The database connection object.
     */
    MMORPGApplication* registerDatabaseConnection(IDatabaseConnection* dbConnection);

    /**
     * @brief This method starts the server and listens for incoming connections.
     * @param port The port number to listen on.
     */
    MMORPGApplication* listen(unsigned short port);

    ~MMORPGApplication();
};