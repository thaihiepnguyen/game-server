#pragma once

#include <memory>
#include <vector>
#include "core/network/tcp_connection.hpp"
#include "core/game_world/queued_packet.hpp"
#include "core/game_world/resource/character/character.hpp"
#include "core/game_world/resource/environment/environment.hpp"
#include "core/util/const.hpp"
#include "protocol/broadcast_packet.hpp"
#include "protocol/end_game_packet.hpp"
#include <chrono>
#include <queue>
#include <mutex>
#include <unordered_map>
#include <condition_variable>

typedef std::unordered_map<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>> player_pool;

class GameRoom // GameWorld
{
public:
    static std::condition_variable cv;
    static std::mutex cv_mtx;

private:
    std::mutex _mtx;
    std::thread _t;

    player_pool _players;
    bool _gameRunning = true;
    std::shared_ptr<IEnvironment> _environment;
    const float _tick = TICK_DURATION; // 20ms per tick
    std::queue<QueuedPacket> _packetQueue;
    float _groundY;
    int _gameTimer = 90;
    float _lastTime = 0.0f;

public:
    GameRoom(player_pool players, std::shared_ptr<IEnvironment> environment)
        : _players(std::move(players)), _environment(environment)
    {
        _groundY = _environment->getGroundYRatio() * WINDOW_HEIGHT;
    }

    std::vector<std::shared_ptr<TCPConnection>> getConnections() const
    {
        std::vector<std::shared_ptr<TCPConnection>> connections;
        for (const auto &player : _players)
        {
            connections.push_back(player.first);
        }
        return connections;
    }

    void start()
    {
        _t = std::thread(&GameRoom::_startGameLoop, this);
    }

    bool isGameRunning() const
    {
        return _gameRunning;
    }

    void enqueuePacket(QueuedPacket packet)
    {
        std::unique_lock<std::mutex> locker(_mtx);
        _packetQueue.push(packet);
    }

private:
    void _startGameLoop();

    void _update(float dt);

    void _processQueuedPackets(float dt);

    void _updateBattle(float dt);

    void _updateCharacterStates(float dt);

    void _updateCollisions();

    void _broadcastUpdate();

    void _updateInteractionBetween(std::shared_ptr<ICharacter> character, std::shared_ptr<ICharacter> opponent);

    bool _isEndGame();

    std::shared_ptr<ICharacter> _getCharacterByConnection(std::shared_ptr<TCPConnection> conn);

    std::vector<std::shared_ptr<ICharacter>> _getCharacters();

public:
    ~GameRoom()
    {
        _gameRunning = false;
        if (_t.joinable())
        {
            _t.join();
        }
    }
};