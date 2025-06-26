#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct RoomDataPacket
{
    int32_t character1; // ID of the first character
    int32_t character2; // ID of the second character
    int32_t background; // Background ID
    int8_t side;        // 0 for left, 1 for right
};
#pragma pack(pop)

#pragma pack(push, 1)
struct RoomPacket : PacketHeader, RoomDataPacket
{
    const char *toBuffer() const
    {
        return reinterpret_cast<const char *>(this);
    }
};
#pragma pack(pop)