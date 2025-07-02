#include "command/join_room_command.hpp"

void JoinRoomCommand::execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length)
{
    _roomService->waitForRoom(connection);
}