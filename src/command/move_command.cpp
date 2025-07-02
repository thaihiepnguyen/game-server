#include "command/move_command.hpp"

void MoveCommand::execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length)
{
    if (length != sizeof(MoveDataPacket))
    {
        // TODO: handle error
        return;
    }

    MoveDataPacket packet;
    std::memcpy(&packet, data, length);

    _inputService->move(connection, packet);
}