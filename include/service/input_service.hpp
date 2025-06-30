#pragma once

#include "core/service/service.hpp"
#include "service/room_service.hpp"
#include "protocol/move_packet.hpp"
#include "protocol/atk_packet.hpp"
#include "protocol/def_packet.hpp"
#include "core/game_world/queued_packet.hpp"
#include "core/util/const.hpp"

class InputService : public IService
{
private:
    std::shared_ptr<RoomService> _roomService;

    void inject(std::shared_ptr<Provider> provider) override
    {
        _roomService = provider->getService<RoomService>();
    }

public:
    void move(const std::shared_ptr<TCPConnection> &connection, const MoveDataPacket &packet)
    {
        auto gameRoom = _roomService->getGameRoomByConnection(connection);
        if (!gameRoom)
        {
            // TODO: handle error
            return;
        }
        QueuedPacket queuePacket;
        queuePacket.connection = connection;
        switch (packet.x)
        {
        case 1:
            queuePacket.packetType = QueuedPacketType::MOVE_LEFT;
            gameRoom->enqueuePacket(queuePacket);
            break;
        case 2:
            queuePacket.packetType = QueuedPacketType::MOVE_RIGHT;
            gameRoom->enqueuePacket(queuePacket);
            break;
        case 3:
            queuePacket.packetType = QueuedPacketType::STOP_MOVE;
            gameRoom->enqueuePacket(queuePacket);
            break;
        default:
            break;
        }
    }

    void atk(const std::shared_ptr<TCPConnection> &connection, const AtkDataPacket &packet)
    {
        auto gameRoom = _roomService->getGameRoomByConnection(connection);
        if (!gameRoom)
        {
            // TODO: handle error
            return;
        }
        QueuedPacket queuePacket;
        queuePacket.connection = connection;
        switch (packet.atk)
        {
        case 1:
            queuePacket.packetType = QueuedPacketType::ATTACK_Z;
            gameRoom->enqueuePacket(queuePacket);
            break;
        case 2:
            queuePacket.packetType = QueuedPacketType::ATTACK_X;
            gameRoom->enqueuePacket(queuePacket);
            break;
        case 3:
            queuePacket.packetType = QueuedPacketType::ATTACK_C;
            gameRoom->enqueuePacket(queuePacket);
            break;
        default:
            break;
        }
    }

    void jump(const std::shared_ptr<TCPConnection> &connection)
    {
        auto gameRoom = _roomService->getGameRoomByConnection(connection);
        if (!gameRoom)
        {
            // TODO: handle error
            return;
        }
        QueuedPacket queuePacket;
        queuePacket.connection = connection;
        queuePacket.packetType = QueuedPacketType::PAC_JUMP;

        gameRoom->enqueuePacket(queuePacket);
    }

    void def(const std::shared_ptr<TCPConnection> &connection, const DefDataPacket &packet)
    {
        auto gameRoom = _roomService->getGameRoomByConnection(connection);
        if (!gameRoom)
        {
            // TODO: handle error
            return;
        }
        QueuedPacket queuePacket;
        queuePacket.connection = connection;
        switch (packet.def)
        {
        case 1:
            queuePacket.packetType = QueuedPacketType::DEFEND;
            gameRoom->enqueuePacket(queuePacket);
            break;
        case 0:
            queuePacket.packetType = QueuedPacketType::UNDEFEND;
            gameRoom->enqueuePacket(queuePacket);
            break;
        default:
            break;
        }
    }
};