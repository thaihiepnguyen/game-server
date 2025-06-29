#include <asio.hpp>
#include <iostream>
#include <memory>
#include "core/network/tcp_server.hpp"
#include "core/fighting_app.hpp"
#include "service/room_service.hpp"
#include "service/resource_service.hpp"
#include "command/join_room_command.hpp"
#include "command/atk_command.hpp"
#include "command/def_command.hpp"
#include "command/jump_command.hpp"
#include "command/move_command.hpp"
#include "service/input_service.hpp"
#include "core/util/const.hpp"

using asio::ip::tcp;

const unsigned short PORT = 8081;

int main()
{
    auto fightingApp = std::make_unique<FightingGameApplication>();
    fightingApp
        ->registerService(new InputService())
        ->registerService(new ResourceService())
        ->registerService(new RoomService())
        ->registerCommand(CommandId::C_ATK, new AtkCommand())
        ->registerCommand(CommandId::C_DEF, new DefCommand())
        ->registerCommand(CommandId::C_JUMP, new JumpCommand())
        ->registerCommand(CommandId::C_MOVE, new MoveCommand())
        ->registerCommand(CommandId::C_WAIT_FOR_MATCH, new JoinRoomCommand());

    std::cout
        << "Server is running on port " << PORT << "\n";

    fightingApp->start(PORT);
    return 0;
}