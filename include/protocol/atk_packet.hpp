#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct AtkDataPacket
{
    int32_t atk; // 1 -> z, 2 -> x, 3 -> c
};
#pragma pack(pop)