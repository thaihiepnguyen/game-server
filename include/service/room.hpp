#include <memory>
#include <vector>
#include "core/network/tcp_connection.hpp"

class Room
{
public:
    std::vector<std::shared_ptr<TCPConnection>> players;

    Room(std::vector<std::shared_ptr<TCPConnection>> playerList) : players(std::move(playerList)) {}
};