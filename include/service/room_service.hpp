#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include "core/service/service.hpp"
#include "network/tcp_connection.hpp"

class Room {
public:
    std::vector<std::shared_ptr<TCPConnection>> players;

    Room(std::vector<std::shared_ptr<TCPConnection>> playerList) : players(std::move(playerList)) {}
};

class RoomService : public IService {
private:
    std::vector<std::shared_ptr<TCPConnection>> _waitingPlayers;
    std::unordered_map<long long, std::shared_ptr<Room>> _rooms;
    unsigned long long _nextRoomId = 0;

    void _createRoom(const std::shared_ptr<TCPConnection>& p1, const std::shared_ptr<TCPConnection>& p2);

public:

    void waitForRoom(const std::shared_ptr<TCPConnection>& connection);
};