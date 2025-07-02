#pragma once

#include "core/command/command.hpp"
#include "service/input_service.hpp"

class JumpCommand : public ICommand
{
private:
    std::shared_ptr<InputService> _inputService;

public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        _inputService = provider->getService<InputService>();
    }

    void execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length);
};
