#include <memory>
#include <vector>
#include "core/network/tcp_connection.hpp"
#include "core/resource/game_room/queued_packet.hpp"
#include "core/resource/character/character.hpp"
#include "core/resource/environment/environment.hpp"
#include "core/util/const.hpp"
#include <chrono>
#include <queue>

class GameRoom // GameWorld
{
private:
    std::vector<std::pair<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>>> _players;
    std::shared_ptr<IEnvironment> _environment; // The environment of the game room
    bool _gameRunning = false;
    const float TICK_DURATION = 0.02f; // 20ms per tick
    std::queue<QueuedPacket> _packetQueue;
    float _groundY;

public:
    GameRoom(std::vector<std::pair<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>>> players, std::shared_ptr<IEnvironment> environment)
        : _players(std::move(players)), _environment(environment) {
        _groundY = _environment->getGroundYRatio() / WINDOW_HEIGHT;
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

    void startGameLoop()
    {
        _gameRunning = true;

        while (_gameRunning)
        {
            _update(TICK_DURATION);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TICK_DURATION * 1000)));
        }
    }

    void stopGameLoop()
    {
        _gameRunning = false;
    }

    void enqueuePacket(const QueuedPacket &packet)
    {
        _packetQueue.push(packet);
    }

private:
    void _update(float dt)
    {
        // process queued packets
        _processQueuedPackets(dt);
        
        // update physics
        _updatePhysics(dt);

        // update character states
        _updateCharacterStates(dt);

    }

    void _processQueuedPackets(float dt)
    {
        while (!_packetQueue.empty())
        {
            QueuedPacket packet = _packetQueue.front();
            auto character = _getCharacterByConnection(packet.connection);

            _packetQueue.pop();
            if (character == nullptr)
            {
                continue;
            }

            switch (packet.packetType)
            {
            case QueuedPacketType::MOVE_LEFT:
                character->setIsMovingLeft(true);
                break;

            case QueuedPacketType::MOVE_RIGHT:
                character->setIsMovingRight(true);
                break;

            case QueuedPacketType::STOP_MOVE:
                character->stopMovement();
                break;

            case QueuedPacketType::JUMP:
                character->jump();
                break;

            case QueuedPacketType::DEFEND:
                character->defend();
                break;

            case QueuedPacketType::UNDEFEND:
                character->undefend();
                break;

            default:
                break;
            }
        }
    }

    void _updatePhysics(float dt)
    {
        for (auto &player : _players)
        {
            auto character = player.second;

            float dy = character->getVelocityY() + GRAVITY;
            character->setVelocityY(dy);
            
            character->setY(character->getY() + dy);

            auto rect = character->getRect();
            if (rect.getBottom() >= _groundY)
            {
                rect.setBottom(_groundY);
                character->setVelocityY(0.0f);
            }
        }
    }

    void _updateCharacterStates(float dt)
    {
        for (auto &player : _players)
        {
            auto character = player.second;

            if (character->getIsMovingLeft())
            {
                character->moveLeft(dt);
            }

            if (character->getIsMovingRight())
            {
                character->moveRight(dt);
            }

            if (character->getState() == CHARACTER_STATES.at("atk_c"))
            {
                if (Time::getCurrentTimeMs() - character->getAtkCTimer() >= character->getAtkCCooldown())
                {
                    character->setState(CHARACTER_STATES.at("idle"));
                }
            }

            if (character->getState() == CHARACTER_STATES.at("atk_x"))
            {
                if (Time::getCurrentTimeMs() - character->getAtkXTimer() >= character->getAtkXCooldown())
                {
                    character->setState(CHARACTER_STATES.at("idle"));
                }
            }

            if (character->getState() == CHARACTER_STATES.at("atk_z"))
            {
                if (Time::getCurrentTimeMs() - character->getAtkZTimer() >= character->getAtkZCooldown())
                {
                    character->setState(CHARACTER_STATES.at("idle"));
                }
            }

            if (character->getState() == CHARACTER_STATES.at("hit"))
            {
                if (Time::getCurrentTimeMs() - character->getHitTimer() >= character->getHitCoolDown())
                {
                    character->setState(CHARACTER_STATES.at("idle"));
                }
            }

            if (character->getState() == CHARACTER_STATES.at("jump"))
            {
                if (character->getIsOnGround())
                {
                    character->setState(CHARACTER_STATES.at("idle"));
                }
            }
        }
    }

    std::shared_ptr<ICharacter> _getCharacterByConnection(std::shared_ptr<TCPConnection> conn)
    {
        for (const auto &player : _players)
        {
            if (player.first == conn)
            {
                return player.second;
            }
        }

        return nullptr;
    }
};