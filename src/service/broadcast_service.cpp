#include "service/broadcast_service.hpp"

void BroadcastService::broadcast(const std::shared_ptr<TCPConnection> &connection, BroadcastDataPacket &dataPacket)
{
    auto opponent = _roomService->getOpponentConnection(connection);
    if (!opponent)
    {
        OpponentOutPacket outPacket;
        outPacket.commandId = CommandId::OPPONENT_OUT;
        outPacket.length = 0; // No additional data needed
        connection->send(outPacket.toBuffer(), sizeof(OpponentOutPacket));
        return;
    }

    BroadcastPacket packet;
    packet.commandId = CommandId::BROADCAST;
    packet.length = sizeof(BroadcastDataPacket);
    packet.x = dataPacket.x;
    packet.y = dataPacket.y;
    packet.hp = dataPacket.hp;
    packet.isFlipped = dataPacket.isFlipped; // 0 for left, 1 for right
    packet.state = dataPacket.state;         // 0 for idle, 1 for attack,

    opponent->send(packet.toBuffer(), sizeof(BroadcastPacket));
}