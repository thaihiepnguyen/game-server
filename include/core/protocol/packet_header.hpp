#pragma once

#include <string>

#pragma pack(push, 1)
struct PacketHeader
{
    int32_t commandId;
    int32_t length;
};
#pragma pack(pop)