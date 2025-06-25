#pragma once

#include "core/protocol/packet_header.hpp"

#pragma pack(push, 1)
struct OpponentOutPacket : PacketHeader
{
    const char *toBuffer() const
    {
        return reinterpret_cast<const char *>(this);
    }
};
#pragma pack(pop)