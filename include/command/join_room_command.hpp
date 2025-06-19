#pragma once

#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "service/room_service.hpp"
#include <iostream>

class JoinRoomCommand : public ICommand
{
private:
    std::shared_ptr<RoomService> _roomService;

public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        _roomService = provider->getService<RoomService>();
    }

    void execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length) override
    {
        _roomService->waitForRoom(connection);
    }
};