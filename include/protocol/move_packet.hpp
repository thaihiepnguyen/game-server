#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct MoveDataPacket
{
    int32_t x; // -1 -> left, 0 -> stop, 1 -> right
};
#pragma pack(pop)