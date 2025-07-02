#pragma once
#include "protocol/arrow_packet.hpp"

#pragma pack(push, 1)
struct FireDataPacket : public ArrowDataPacket
{
};
#pragma pack(pop)

#pragma pack(push, 1)
class FirePacket : public ArrowPacket
{
};

#pragma pack(pop)