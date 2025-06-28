#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct DefDataPacket
{
    int32_t def; // 1 -> def, 0 -> undef
};
#pragma pack(pop)