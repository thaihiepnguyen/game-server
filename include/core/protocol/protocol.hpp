#pragma once

#include "core/protocol/packet_header.hpp"
#include <stdexcept>

class Protocol
{
public:
    static PacketHeader getHeader(const char *data, std::size_t length);
};