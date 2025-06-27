#include <memory>
#include <vector>
#include "core/network/tcp_connection.hpp"
#include "core/resource/game_room/queued_packet.hpp"
#include "core/resource/character/character.hpp"
#include "core/resource/environment/environment.hpp"
#include "core/util/const.hpp"
#include "protocol/broadcast_packet.hpp"
#include <chrono>
#include <queue>
#include <mutex>

class GameRoom // GameWorld
{
private:
    std::mutex _mtx;
    std::thread _t;

    std::vector<std::pair<std::shared_ptr<TCPConnection>, std::shared_ptr<ICharacter>>> _players;
    std::shared_ptr<IEnvironment> _environment;
    bool _gameRunning = true;
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

    void start()
    {
        _t = std::thread(&GameRoom::_startGameLoop, this);
    }

    void stop()
    {
        _gameRunning = false;
        _t.join();
    }

    void enqueuePacket(QueuedPacket packet)
    {
        std::unique_lock<std::mutex> locker(_mtx);
        _packetQueue.push(packet);
    }

private:
    void _startGameLoop()
    {
        while (_gameRunning)
        {
            _update(TICK_DURATION);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TICK_DURATION * 1000)));
        }
    }

    void _update(float dt)
    {
        // process queued packets
        _processQueuedPackets(dt);
        
        // update physics
        _updatePhysics(dt);

        // update character states
        _updateCharacterStates(dt);

        // update collisions
        _updateCollisions();

        // broadcast
        _broadcastUpdate();
    }

    void _processQueuedPackets(float dt)
    {
        std::unique_lock<std::mutex> locker(_mtx);
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

            case QueuedPacketType::ATTACK_Z:
                character->attackZ();
                break;

            case QueuedPacketType::ATTACK_X:
                character->attackX();
                break;

            case QueuedPacketType::ATTACK_C:
                character->attackC();
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
            int currentState = character->getState();

            if (character->getIsMovingLeft())
            {
                character->moveLeft(dt);
            }

            if (character->getIsMovingRight())
            {
                character->moveRight(dt);
            }

            if (currentState == CharacterState::ATK_C)
            {
                if (Time::getCurrentTimeMs() - character->getAtkCTimer() >= character->getAtkCCooldown())
                {
                    character->setState(CharacterState::IDLE);
                }
            }

            if (currentState == CharacterState::ATK_X)
            {
                if (Time::getCurrentTimeMs() - character->getAtkXTimer() >= character->getAtkXCooldown())
                {
                    character->setState(CharacterState::IDLE);
                }
            }

            if (currentState == CharacterState::ATK_Z)
            {
                if (Time::getCurrentTimeMs() - character->getAtkZTimer() >= character->getAtkZCooldown())
                {
                    character->setState(CharacterState::IDLE);
                }
            }

            if (currentState == CharacterState::HIT)
            {
                if (Time::getCurrentTimeMs() - character->getHitTimer() >= character->getHitCoolDown())
                {
                    character->setState(CharacterState::IDLE);
                }
            }

            if (currentState == CharacterState::JUMP)
            {
                if (character->getIsOnGround())
                {
                    character->setState(CharacterState::IDLE);
                }
            }
        }
    }

    void _updateCollisions()
    {
        auto [charA, charB] = _getCharacters();
        
        _updateInteractBetween(charA, charB);
        _updateInteractBetween(charB, charA);
    }

    void _broadcastUpdate()
    {
        auto connections = getConnections();
        for (const auto &player : _players)
        {
            auto character = player.second;

            BroadcastPacket packet;
            packet.commandId = CommandId::BROADCAST;
            packet.length = sizeof(BroadcastDataPacket);
            packet.hp = character->getHp();
            packet.x = character->getX();
            packet.y = character->getY();
            packet.state = character->getState();

            for (const auto &conn : connections)
            {
                conn->send(packet.toBuffer(), sizeof(BroadcastPacket));
            }
        }
    }

    void _updateInteractBetween(std::shared_ptr<ICharacter> character, std::shared_ptr<ICharacter> opponent)
    {
        auto attackRect = character->getAttackRect();

        if (attackRect != nullptr && attackRect->collidesWith(opponent->getRect()) && !opponent->getIsDefending())
        {
            std::shared_ptr<Rect> intersection(attackRect->clip(opponent->getRect()));
            
            if (intersection != nullptr)
            {
                float damgeRatio = intersection->getWidth() / opponent->getRect().getWidth();
                opponent->hit((int)damgeRatio);
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

    std::pair<std::shared_ptr<ICharacter>, std::shared_ptr<ICharacter>> _getCharacters()
    {
        auto charA = _players[0].second;
        auto charB = _players[1].second;

        return { charA, charB };
    }

    ~GameRoom() {
        if (_t.joinable()) {
            _gameRunning = false;
            _t.join();
        }
    }
};