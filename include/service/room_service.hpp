#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "core/service/service.hpp"
#include "core/network/tcp_connection.hpp"
#include "core/game_world/game_room.hpp"
#include "core/protocol/packet_header.hpp"
#include "protocol/room_packet.hpp"
#include "service/resource_service.hpp"
#include "core/service/provider.hpp"
#include <thread>

class RoomService : public IService
{
private:
    std::shared_ptr<ResourceService> _resourceService;
    std::vector<std::shared_ptr<TCPConnection>> _connections;
    std::unordered_map<long long, std::shared_ptr<GameRoom>> _rooms;
    unsigned long long _nextRoomId = 0; // TODO: use uuid for this variable.

private:
    void _createRoom(const std::shared_ptr<TCPConnection> &player1, const std::shared_ptr<TCPConnection> &player2);

    void _notifyRoomCreated(
        const std::shared_ptr<TCPConnection> &connection,
        const int &character,
        const int &opponent,
        const int &background,
        const std::string &side);

    void _removeConnectionFromWaiting(const std::shared_ptr<TCPConnection> &connection);

    void _cleanRoom();

    void inject(std::shared_ptr<Provider> provider) override
    {
        _resourceService = provider->getService<ResourceService>();
    }

public:
    RoomService()
    {
        std::thread t(&RoomService::_cleanRoom, this);
        t.detach();
    }

    void waitForRoom(const std::shared_ptr<TCPConnection> &connection);

    long long getRoomIdByConnection(const std::shared_ptr<TCPConnection> &connection) const;

    std::shared_ptr<TCPConnection> getOpponentConnection(const std::shared_ptr<TCPConnection> &connection) const;

    std::shared_ptr<GameRoom> getGameRoomByConnection(const std::shared_ptr<TCPConnection> &connection) const;
};