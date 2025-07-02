#include "command/def_command.hpp"

void DefCommand::execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length)
{
    if (length != sizeof(DefDataPacket))
    {
        // TODO: handle error
        return;
    }

    DefDataPacket packet;
    std::memcpy(&packet, data, length);

    _inputService->def(connection, packet);
}