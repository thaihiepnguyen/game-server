#pragma once

#include "core/command/command.hpp"
#include "protocol/atk_packet.hpp"
#include "service/input_service.hpp"

class AtkCommand : public ICommand
{
private:
    std::shared_ptr<InputService> _inputService;
    
public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        _inputService = provider->getService<InputService>();
    }

    void execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length) override
    {
        if (length != sizeof(AtkDataPacket)) {
            // TODO: handle error
            return;
        }

        AtkDataPacket packet;
        std::memcpy(&packet, data, length);

        _inputService->atk(connection, packet);
    }
};
