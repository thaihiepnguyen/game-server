#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include "core/service/service.hpp"
#include "core/network/tcp_connection.hpp"
#include "service/room.hpp"
#include "core/protocol/packet_header.hpp"
#include "protocol/room_packet.hpp"

class RoomService : public IService
{
private:
    std::queue<std::shared_ptr<TCPConnection>> _connectionQueue;
    std::unordered_map<long long, std::shared_ptr<Room>> _rooms;
    unsigned long long _nextRoomId = 0;

public:
    void inject(std::shared_ptr<Provider> provider) override
    {
        // This method can be used to inject dependencies if needed
    }

    void _notifyRoomCreated(const std::shared_ptr<Room> &room, int roomId)
    {
        // Notify all players in the room about its creation
        for (const auto &player : room->players)
        {
            PacketHeader header;
            header.commandId = 1;              // Example command ID for room creation
            header.length = sizeof(long long); // Assuming we send the room ID
            RoomPacket packet;
            packet.header = header;
            packet.roomId = roomId;
            player->send(reinterpret_cast<const char *>(&packet), sizeof(RoomPacket));
        }
    }

    void waitForRoom(const std::shared_ptr<TCPConnection> &connection)
    {
        if (_connectionQueue.empty())
        {
            // If no room is available, create a new one
            _connectionQueue.push(connection);
        }
        else
        {
            // If a room is available, join it
            auto opponent = _connectionQueue.front();
            _connectionQueue.pop();
            _rooms[_nextRoomId++] = std::make_shared<Room>(std::vector{connection, opponent});
        }
    }
};