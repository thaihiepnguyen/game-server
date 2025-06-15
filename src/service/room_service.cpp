#include "service/room_service.hpp"



void RoomService::_createRoom(const std::shared_ptr<TCPConnection>& p1, const std::shared_ptr<TCPConnection>& p2) {
    std::vector<std::shared_ptr<TCPConnection>> players = {p1, p2};
    auto room = std::make_shared<Room>(players);

    long long roomId = _nextRoomId++;
    _rooms[roomId] = room;
    std::cout << "Room created with ID: " << roomId << " for players: "
                << p1->userId() << " and " << p2->userId() << std::endl;
    for (const auto& player : room->players) {
        std::cout << "Room created with ID: " << roomId << " for players: "
                    << p1->userId() << " and " << p2->userId() << std::endl;
        player->send(Protocol::Packet(Protocol::Command::WAIT_FOR_ROOM, {
            {"roomId", Protocol::Value(roomId)},
            {"opponentId", Protocol::Value(p2->userId())}
        }).toString());
    }
}

void RoomService::waitForRoom(const std::shared_ptr<TCPConnection>& connection) {
    _waitingPlayers.push_back(connection);
    
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