#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct BroadcastDataPacket
{
    int32_t x_c;
    int32_t y_c;
    int32_t hp_c;
    int32_t state_c;    // 0 for idle, 1 for attack, etc.
    int32_t x_o;
    int32_t y_o;
    int32_t hp_o;
    int32_t state_o;    // 0 for idle, 1 for attack, etc.
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BroadcastPacket : PacketHeader, BroadcastDataPacket
{
    const char *toBuffer() const
    {
        return reinterpret_cast<const char *>(this);
    }
};
#pragma pack(pop)