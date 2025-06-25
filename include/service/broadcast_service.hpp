#pragma once

#include "core/service/provider.hpp"
#include "core/service/service.hpp"
#include "core/network/tcp_connection.hpp"
#include "service/room_service.hpp"
#include "core/utils/const.hpp"
#include "core/protocol/packet_header.hpp"
#include "protocol/broadcast_packet.hpp"
#include "protocol/opponent_out_packet.hpp"
#include <memory>

class BroadcastService : public IService
{
private:
    std::shared_ptr<RoomService> _roomService;

public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        _roomService = provider->getService<RoomService>();
    }

    void broadcast(const std::shared_ptr<TCPConnection> &connection, BroadcastDataPacket &dataPacket);
};