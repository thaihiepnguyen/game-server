#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct EndGameDataPacket
{
    int32_t result; // 2 is lost, 1 is win, 3 is draw
};
#pragma pack(pop)

#pragma pack(push, 1)
struct EndGamePacket : PacketHeader, EndGameDataPacket
{
    const char *toBuffer() const
    {
        return reinterpret_cast<const char *>(this);
    }
};
#pragma pack(pop)