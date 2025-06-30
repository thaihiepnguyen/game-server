#pragma once

#include "core/command/command.hpp"
#include "protocol/def_packet.hpp"
#include "service/input_service.hpp"

class DefCommand : public ICommand
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
        if (length != sizeof(DefDataPacket)) {
            // TODO: handle error
            return;
        }

        DefDataPacket packet;
        std::memcpy(&packet, data, length);

        _inputService->def(connection, packet);
    }
};
