#include "command/atk_command.hpp"

void AtkCommand::execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length)
{
    if (length != sizeof(AtkDataPacket))
    {
        // TODO: handle error
        return;
    }

    AtkDataPacket packet;
    std::memcpy(&packet, data, length);

    _inputService->atk(connection, packet);
}