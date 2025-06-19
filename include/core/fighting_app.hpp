#pragma once

#include <string>
#include "core/command/command.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "core/network/tcp_connection.hpp"
#include "core/network/tcp_server.hpp"
#include "core/service/provider.hpp"
#include "core/service/service.hpp"
#include "core/utils/log.hpp"

class FightingGameApplication
{
private:
    std::shared_ptr<Provider> _provider;
    std::unordered_map<int, std::shared_ptr<ICommand>> _commands;
    void _handleCommand(const std::shared_ptr<TCPConnection> &connection, int commandId, const char *data, std::size_t length);

public:
    FightingGameApplication();

    FightingGameApplication *registerCommand(int commandId, ICommand *command);

    FightingGameApplication *registerService(IService *service);

    /**
     * @brief This method starts the server and listens for incoming connections.
     * @param port The port number to listen on.
     */
    void start(unsigned short port);
};