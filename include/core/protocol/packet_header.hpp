#pragma once

#include <string>
#include <stdint.h>

#pragma pack(push, 1)
struct PacketHeader
{
    int32_t commandId;
    int32_t length;
};
#pragma pack(pop)