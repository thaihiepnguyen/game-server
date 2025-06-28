#include "service/room_service.hpp"
#include "core/resource/character/character.hpp"
#include <unordered_map>
#include "core/util/const.hpp"
#include <thread>

void RoomService::_createRoom(const std::shared_ptr<TCPConnection> &player1, const std::shared_ptr<TCPConnection> &player2)
{
    // Create a new room with the given connections
    long long roomId = ++_nextRoomId;

    // Assign random characters and backgrounds to both players
    int character1 = _resourceService->getRandomCharacterId();
    int character2 = _resourceService->getRandomCharacterId();
    int background = _resourceService->getRandomBackgroundId();

    std::shared_ptr<ICharacter> character1Ptr(_resourceService->createCharacter(character1, 100, 200, false));
    std::shared_ptr<ICharacter> character2Ptr(_resourceService->createCharacter(character2, WINDOW_WIDTH - 100, 200, true));

    std::unordered_map<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>> players;
    
    players[player1] = character1Ptr;
    players[player2] = character2Ptr;

    // Create the environment for the game room
    std::shared_ptr<IEnvironment> environment(_resourceService->createEnvironment(background));

    _rooms[roomId] = std::make_shared<GameRoom>(players, environment);
    _rooms[roomId]->start();

    for (const auto &player : players)
    {
        player.first->events.subscribe("disconnect", [this, conn = player.first, roomId]()
                                    {
                                        // TODO: consider to notify another of disconnect. 
                                        _rooms.erase(roomId); 
                                    });
    }

    // Notify both players about the room creation
    _notifyRoomCreated(player1, character1, character2, background, "left");
    _notifyRoomCreated(player2, character2, character1, background, "right");
}

void RoomService::_notifyRoomCreated(
    const std::shared_ptr<TCPConnection> &connection,
    const int &character,
    const int &opponent,
    const int &background,
    const std::string &side = "left")
{
    RoomPacket packet;
    packet.commandId = CommandId::C_WAIT_FOR_MATCH;
    packet.length = sizeof(RoomDataPacket);
    packet.character1 = character;
    packet.character2 = opponent;
    packet.background = background;
    packet.side = (side == "left") ? 0 : 1; // false for left, true for right
    connection->send(packet.toBuffer(), sizeof(RoomPacket));
}

void RoomService::_removeConnection(const std::shared_ptr<TCPConnection> &connection)
{
    auto it = std::find_if(_connections.begin(), _connections.end(),
                           [&connection](const std::shared_ptr<TCPConnection> &conn)
                           {
                               return conn == connection;
                           });
    if (it != _connections.end())
    {
        _connections.erase(it);
    }
}

void RoomService::waitForRoom(const std::shared_ptr<TCPConnection> &connection)
{
    if (_connections.size() < 1)
    {
        // If no room is available, create a new one
        _connections.push_back(connection);
        connection->events.subscribe("disconnect", [this, &connection]()
                                     { _removeConnection(connection); });
    }
    else
    {
        // If a room is available, join it
        auto opponent = _connections.back();
        _removeConnection(opponent);
        _createRoom(connection, opponent);
    }
}

long long RoomService::getRoomIdByConnection(const std::shared_ptr<TCPConnection> &connection) const
{
    for (const auto &room : _rooms)
    {
        const auto &players = room.second->getConnections();
        if (std::find(players.begin(), players.end(), connection) != players.end())
        {
            return room.first; // Return the room ID if the connection is found
        }
    }
    return -1; // Return -1 if the connection is not found in any room
}

std::shared_ptr<TCPConnection> RoomService::getOpponentConnection(const std::shared_ptr<TCPConnection> &connection) const
{
    long long roomId = getRoomIdByConnection(connection);
    if (roomId == -1)
    {
        return nullptr; // Connection not found in any room
    }

    const auto &players = _rooms.at(roomId)->getConnections();
    for (const auto &player : players)
    {
        if (player != connection) // Check if the player is not the same as the connection
        {
            return player; // Return the opponent connection
        }
    }
    return nullptr; // No opponent found
}

std::shared_ptr<GameRoom> RoomService::getGameRoomByConnection(const std::shared_ptr<TCPConnection> &connection) const
{
    long long roomId = getRoomIdByConnection(connection);
    auto it = _rooms.find(roomId);
    if (it != _rooms.end()) {
        return it->second;
    }
    return nullptr;
}


void RoomService::_cleanRoom()
{
    while (1)
    {
        std::unique_lock<std::mutex> locker(GameRoom::cv_mtx);
        GameRoom::cv.wait(locker);

        for (auto it = _rooms.begin(); it != _rooms.end(); )
        {
            if (!it->second->isGameRunning())
            {
                it = _rooms.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}