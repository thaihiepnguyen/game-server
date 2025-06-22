#include <asio.hpp>
#include <iostream>
#include <memory>
#include "core/network/tcp_server.hpp"
#include "core/fighting_app.hpp"
#include "service/room_service.hpp"
#include "service/resource_service.hpp"
#include "service/broadcast_service.hpp"
#include "command/join_room_command.hpp"
#include "command/broadcast_command.hpp"
#include "core/utils/const.hpp"

using asio::ip::tcp;

const unsigned short PORT = 8081;

int main()
{
    // Create the fighting game application
    auto fightingApp = std::make_unique<FightingGameApplication>();
    fightingApp
        ->registerService(new ResourceService())
        ->registerService(new RoomService())
        ->registerService(new BroadcastService())
        ->registerCommand(CommandId::WAIT_FOR_MATCH, new JoinRoomCommand())
        ->registerCommand(CommandId::BROADCAST, new BroadcastCommand());

    std::cout
        << "Server is running on port " << PORT << "\n";
    // Start the server
    fightingApp->start(PORT);
    return 0;
}