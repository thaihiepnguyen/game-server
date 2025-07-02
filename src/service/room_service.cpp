#include "service/room_service.hpp"
#include "core/game_world/resource/character/character.hpp"
#include <unordered_map>
#include "core/util/const.hpp"
#include "protocol/opponent_out_packet.hpp"
#include <thread>

void RoomService::_createRoom(const std::shared_ptr<TCPConnection> &player1, const std::shared_ptr<TCPConnection> &player2)
{
    // Create a new room with the given connections
    long long roomId = ++_nextRoomId;

    // Assign random characters and environments to both players
    int character1 = _resourceService->getRandomCharacterId();
    int character2 = _resourceService->getRandomCharacterId();
    int background = _resourceService->getRandomBackgroundId();

    std::shared_ptr<ICharacter> character1Ptr(_resourceService->createCharacter(character1, 100, 200));
    std::shared_ptr<ICharacter> character2Ptr(_resourceService->createCharacter(character2, WINDOW_WIDTH - CHARACTER_WIDTH - 100, 200));

    std::unordered_map<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>> players;

    players[player1] = character1Ptr;
    players[player2] = character2Ptr;

    // Create the environment for the game room
    std::shared_ptr<IEnvironment> environment(_resourceService->createEnvironment(background));

    _rooms[roomId] = std::make_shared<GameRoom>(players, environment);

    for (const auto &player : players)
    {
        auto conn = player.first;
        std::weak_ptr<TCPConnection> weakConn = conn;
        conn->events.subscribe("disconnect", [this, roomId]()
                               {
                            auto roomIt = _rooms.find(roomId);
                            if (roomIt != _rooms.end()) {
                                OpponentOutPacket packet;
                                packet.commandId = CommandId::C_OPPONENT_OUT;
                                packet.length = 0;
                                
                                auto connections = roomIt->second->getConnections();
                                for (auto& connection : connections) {
                                    if (!connection->isClosed()) {
                                        connection->send(packet.toBuffer(), sizeof(OpponentOutPacket));
                                    }
                                }
                                
                                _rooms.erase(roomIt);
                            } });
    }

    // Notify both players about the room creation
    _notifyRoomCreated(player1, character1, character2, background, "left");
    _notifyRoomCreated(player2, character2, character1, background, "right");

    // Start game loop
    _rooms[roomId]->start();
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

void RoomService::_removeConnectionFromWaiting(const std::shared_ptr<TCPConnection> &connection)
{
    if (!connection)
        return;

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
        std::weak_ptr<TCPConnection> weakConn = connection;
        connection->events.subscribe("disconnect", [this, weakConn]()
                                     { _removeConnectionFromWaiting(weakConn.lock()); });
    }
    else
    {
        // If a room is available, join it
        auto opponent = _connections.back();
        _removeConnectionFromWaiting(opponent);
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
            return room.first;
        }
    }
    return -1;
}

std::shared_ptr<TCPConnection> RoomService::getOpponentConnection(const std::shared_ptr<TCPConnection> &connection) const
{
    long long roomId = getRoomIdByConnection(connection);
    if (roomId == -1)
    {
        return nullptr;
    }

    const auto &players = _rooms.at(roomId)->getConnections();
    for (const auto &player : players)
    {
        if (player != connection)
        {
            return player;
        }
    }
    return nullptr;
}

std::shared_ptr<GameRoom> RoomService::getGameRoomByConnection(const std::shared_ptr<TCPConnection> &connection) const
{
    long long roomId = getRoomIdByConnection(connection);
    auto it = _rooms.find(roomId);
    if (it != _rooms.end())
    {
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

        for (auto it = _rooms.begin(); it != _rooms.end();)
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