#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct BroadcastDataPacket
{
    int32_t x;
    int32_t y;
    int32_t hp;
    int8_t isFlipped; // 0 for left, 1 for right
    int32_t state; // 0 for idle, 1 for attack, etc.
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BroadcastPacket : PacketHeader, BroadcastDataPacket
{
    const char* toBuffer() const
    {
        return reinterpret_cast<const char*>(this);
    }
};
#pragma pack(pop)