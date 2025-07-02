#include "core/fighting_app.hpp"
#include "core/protocol/packet_header.hpp"
#include "core/network/tcp_server.hpp"
#include "core/protocol/protocol.hpp"

FightingGameApplication::FightingGameApplication()
{
    _provider = std::make_shared<Provider>();
}

void FightingGameApplication::_buildRelationship()
{
    for (auto &command : _commands)
    {
        command.second->inject(_provider);
    }

    for (auto &service : _provider->_services)
    {
        service.second->inject(_provider);
    }
}

void FightingGameApplication::_handleCommand(const std::shared_ptr<TCPConnection> &connection, int commandId, const char *data, std::size_t length)
{
    auto it = _commands.find(commandId);
    if (it != _commands.end())
    {
        it->second->execute(connection, data, length);
    }
    else
    {
        Logger::logError("Command not found: " + std::to_string(commandId));
    }
}

FightingGameApplication *FightingGameApplication::registerCommand(int commandId, ICommand *command)
{
    std::shared_ptr<ICommand> cmdPtr(command);
    _commands[commandId] = cmdPtr;

    return this;
}

FightingGameApplication *FightingGameApplication::registerService(IService *service)
{
    std::shared_ptr<IService> servicePtr(service);
    _provider->addService(servicePtr);

    return this;
}

void FightingGameApplication::start(unsigned short port)
{
    _buildRelationship();
    auto server = std::make_shared<TCPServer>();

    if (server->bind(port))
    {
        server->listen();

        server->accept(
            [&](auto connection, const char *msg, std::size_t length)
            {
                PacketHeader header = Protocol::getHeader(msg, length);

                // jump to the data part of the message
                const char *data = msg + sizeof(PacketHeader);

                _handleCommand(connection, header.commandId, data, header.length);
            });
    }

    server->run();
}