#pragma once

#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "core/utils/log.hpp"
#include "service/broadcast_service.hpp"
#include "protocol/broadcast_packet.hpp"
#include <iostream>
#include <cstring>

class BroadcastCommand : public ICommand
{
private:
    std::shared_ptr<BroadcastService> _broadcastService;

public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        _broadcastService = provider->getService<BroadcastService>();
    }

    void execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length) override
    {
        BroadcastDataPacket dataPacket;
        if (length != sizeof(BroadcastDataPacket))
        {
            Logger::logError("Data length is different for BroadcastDataPacket.");
            return;
        }
        memcpy(&dataPacket, data, sizeof(BroadcastDataPacket));

        _broadcastService->broadcast(connection, dataPacket);
    }
};