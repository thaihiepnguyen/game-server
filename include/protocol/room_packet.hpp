#include "core/protocol/packet.hpp"

struct RoomPacket : Packet
{
    long long roomId;
};