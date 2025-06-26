#pragma once

#include "core/network/tcp_connection.hpp"

struct QueuedPacket
{
    const std::shared_ptr<TCPConnection> &connection; // which connection this packet is for
    const int packetType;                             // type of packet, e.g., attack, move, etc.
};
