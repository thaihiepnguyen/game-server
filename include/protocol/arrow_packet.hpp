#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct ArrowDataPacket
{
    int8_t owner; // 1 for owner, 0 is not 
    int32_t x;
    int32_t y;
    int8_t direction; // 0 for left, 1 for right
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ArrowPacket : PacketHeader, ArrowDataPacket
{
    const char *toBuffer() const
    {
        return reinterpret_cast<const char *>(this);
    }
};
#pragma pack(pop)