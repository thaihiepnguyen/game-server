#include <asio.hpp>
#include <iostream>
#include <memory>
#include "core/network/tcp_server.hpp"
#include "core/fighting_app.hpp"
#include "service/room_service.hpp"
#include "command/join_room_command.hpp"

using asio::ip::tcp;

const unsigned short PORT = 8080;

int main()
{
    // Create the fighting game application
    auto fightingApp = std::make_unique<FightingGameApplication>();
    fightingApp
        ->registerService(new RoomService())
        ->registerCommand(1, new JoinRoomCommand());

    std::cout
        << "Server is running on port " << PORT << "\n";
    // Run the server
    fightingApp->start(PORT);
    return 0;
}