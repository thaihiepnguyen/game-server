#pragma once

#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "service/room_service.hpp"


class RoomCommand : public ICommand {
private:
    std::shared_ptr<RoomService> _roomService;

public:
    void inject(std::shared_ptr<Provider> provider) override {
        _roomService = provider->getService<RoomService>();
    }

    void execute(
        const std::shared_ptr<TCPConnection>& connection,
        Protocol::Packet& packet
    ) override {
        if (connection->userId() == 0) {
            throw std::runtime_error("Connection user ID is not set");
        }

        // Call the room service to wait for a room
        _roomService->waitForRoom(connection);
    }
};