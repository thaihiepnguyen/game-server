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

    void _createRoom(const std::shared_ptr<TCPConnection>& p1, const std::shared_ptr<TCPConnection>& p2) {
        std::vector<std::shared_ptr<TCPConnection>> players = {p1, p2};
        auto room = std::make_shared<Room>(players);

        long long roomId = _nextRoomId++;
        _rooms[roomId] = room;
        std::cout << "Room created with ID: " << roomId << " for players: "
                  << p1->userId() << " and " << p2->userId() << std::endl;
        for (const auto& player : room->players) {
            player->addOnDisconnectListener([this, room, roomId](std::shared_ptr<TCPConnection> conn) {
                std::cout << "Player " << conn->userId() << " disconnected from room ID: " << roomId << std::endl;
                _rooms.erase(roomId);
            });
            std::cout << "Room created with ID: " << roomId << " for players: "
                      << p1->userId() << " and " << p2->userId() << std::endl;
            player->writeMessage(Protocol::Packet(Protocol::Command::WAIT_FOR_ROOM, {
                {"roomId", Protocol::Value(roomId)},
                {"opponentId", Protocol::Value(p2->userId())}
            }).toString());
        }
    }

public:

    void waitForRoom(const std::shared_ptr<TCPConnection>& connection) {
        _waitingPlayers.push_back(connection);
        connection->addOnDisconnectListener([this](std::shared_ptr<TCPConnection> conn) {
            // Remove the connection from waiting players if it disconnects
            auto it = std::find(_waitingPlayers.begin(), _waitingPlayers.end(), conn);
            if (it != _waitingPlayers.end()) {
                _waitingPlayers.erase(it);
            }
        });
        
        std::shared_ptr<TCPConnection> opponent;
        for (const auto& player : _waitingPlayers) {
            if (player->userId() != connection->userId()) {
                opponent = player;
                break;
            }
        }

        if (opponent) {
            // Create a room with the two players
            std::cout << "Creating room for players: " 
                      << connection->userId() << " and " << opponent->userId() << std::endl;
            _createRoom(connection, opponent);

            _waitingPlayers.erase(std::remove(_waitingPlayers.begin(), _waitingPlayers.end(), connection), _waitingPlayers.end());
            _waitingPlayers.erase(std::remove(_waitingPlayers.begin(), _waitingPlayers.end(), opponent), _waitingPlayers.end());
        }
    }
};