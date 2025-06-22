#include "core/protocol/protocol.hpp"

PacketHeader Protocol::getHeader(const char *data, std::size_t length)
{
    if (length < sizeof(PacketHeader))
    {
        throw std::runtime_error("Data length is too short for PacketHeader");
    }

    PacketHeader header;
    memcpy(&header.commandId, data, sizeof(int));
    memcpy(&header.length, data + sizeof(int), sizeof(int));

    return header;
}