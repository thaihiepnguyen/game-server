#pragma once

#include <memory>
#include <vector>
#include "core/network/tcp_connection.hpp"
#include "core/resource/game_room/queued_packet.hpp"
#include "core/resource/character/character.hpp"
#include "core/resource/environment/environment.hpp"
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
        : _players(std::move(players)), _environment(environment) {
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
    void _startGameLoop()
    {
        while (_gameRunning)
        {
            _update(_tick);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(_tick * 1000)));
        }
    }

    void _update(float dt)
    {
        // process queued packets
        _processQueuedPackets(dt);
        
        // update battle
        _updateBattle(dt);

        // update character states
        _updateCharacterStates(dt);

        // update collisions
        _updateCollisions();
        
        // broadcast
        _broadcastUpdate();

        // check end game
        if (_isEndGame())
        {
            _gameRunning = false;
            std::unique_lock<std::mutex> locker(cv_mtx);
            locker.unlock();
            cv.notify_one();
            return;
        }
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

            case QueuedPacketType::PAC_JUMP:
                character->jump(_groundY);
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

    void _updateBattle(float dt)
    {
        for (auto &player : _players)
        {
            auto character = player.second;

            float dy = character->getVelocityY() + GRAVITY * 50 * dt;
            character->setVelocityY(dy);
            
            character->setY(character->getY() + dy);

            auto rect = character->getRect();

            if (rect->getBottom() >= _groundY)
            {
                rect->setBottom(_groundY);
                character->setVelocityY(0.0f);
            }
        }

        if (Time::getCurrentTimeMs() - _lastTime >= 1000)
        {
            _gameTimer -= 1;
            _lastTime = Time::getCurrentTimeMs();
        }
    }

    void _updateCharacterStates(float dt)
    {
        auto characters = _getCharacters();
        characters[0]->lookAt(characters[1].get());
        characters[1]->lookAt(characters[0].get());
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
                if (character->getIsOnGround(_groundY))
                {
                    character->setState(CharacterState::IDLE);
                }
            }
        }
    }

    void _updateCollisions()
    {
        auto characters = _getCharacters();
        if (characters.size() < 2)
        {
            // TODO: handle error
            return;
        }
        
        _updateInteractionBetween(characters[0], characters[1]);
        _updateInteractionBetween(characters[1], characters[0]);
    }

    void _broadcastUpdate()
    {
        auto connections = getConnections();
        auto characters = _getCharacters();

        if (characters.size() != 2)
        {
            // TODO: handle error
            return;
        }

        if (connections.size() != 2)
        {
            // TODO: handle error
            return;
        }

        for (int i = 0; i < 2; ++i)
        {
            BroadcastPacket packet;
            packet.commandId = CommandId::C_BROADCAST;
            packet.length = sizeof(BroadcastDataPacket);

            // Current player
            packet.hp_c = characters[i]->getHp();
            packet.x_c = characters[i]->getX();
            packet.y_c = characters[i]->getY();
            packet.state_c = characters[i]->getState();

            // Opponent
            int opp = 1 - i;
            packet.hp_o = characters[opp]->getHp();
            packet.x_o = characters[opp]->getX();
            packet.y_o = characters[opp]->getY();
            packet.state_o = characters[opp]->getState();

            connections[i]->send(packet.toBuffer(), sizeof(BroadcastPacket));
        }
    }

    void _updateInteractionBetween(std::shared_ptr<ICharacter> character, std::shared_ptr<ICharacter> opponent)
    {
        auto attackRect = character->getAttackRect();

        if (attackRect != nullptr && attackRect->collidesWith(opponent->getRect()) && !opponent->getIsDefending())
        {
            // std::shared_ptr<Rect> intersection(attackRect->clip(opponent->getRect()));
            
            // if (intersection != nullptr)
            // {
            //     float damgeRatio = intersection->getWidth() / opponent->getRect()->getWidth();
            //     opponent->hit((int)damgeRatio);
            // }
            float damgeRatio = character->getAttackDamage();
            opponent->hit((int)damgeRatio);
        }
    }

    bool _isEndGame()
    {
        auto characters = _getCharacters();
        auto connections = getConnections();

        if (characters.size() != 2)
        {
            // TODO: handle error
            return true;
        }

        if (connections.size() != 2)
        {
            // TODO: handle error
            return true;
        }

        if (_gameTimer == 0)
        {
            if (characters[0]->getHp() == characters[1]->getHp())
            {
                EndGamePacket packet;
                packet.commandId = CommandId::C_END_GAME;
                packet.length = sizeof(EndGameDataPacket);

                packet.result = 3;
                
                for (const auto &conn : connections)
                {
                    conn->send(packet.toBuffer(), sizeof(EndGamePacket));
                }

                return true;
            }
            else if (characters[0]->getHp() > characters[1]->getHp())
            {
                EndGamePacket packet;
                packet.commandId = CommandId::C_END_GAME;
                packet.length = sizeof(EndGameDataPacket);

                packet.result = 1; // win
                connections[0]->send(packet.toBuffer(), sizeof(EndGamePacket));
                packet.result = 2; // lost
                connections[1]->send(packet.toBuffer(), sizeof(EndGamePacket));

                return true;
            }
            else
            {
                EndGamePacket packet;
                packet.commandId = CommandId::C_END_GAME;
                packet.length = sizeof(EndGameDataPacket);

                packet.result = 1; // win
                connections[1]->send(packet.toBuffer(), sizeof(EndGamePacket));
                packet.result = 2; // lost
                connections[0]->send(packet.toBuffer(), sizeof(EndGamePacket));

                return true;
            }
        }


        // The match drew!
        if (!characters[0]->getIsAlive() && !characters[1]->getIsAlive())
        {
            EndGamePacket packet;
            packet.commandId = CommandId::C_END_GAME;
            packet.length = sizeof(EndGameDataPacket);

            packet.result = 3;
            
            for (const auto &conn : connections)
            {
                conn->send(packet.toBuffer(), sizeof(EndGamePacket));
            }

            return true;
        }

        // Player 1 win
        if (characters[0]->getIsAlive() && !characters[1]->getIsAlive())
        {
            EndGamePacket packet;
            packet.commandId = CommandId::C_END_GAME;
            packet.length = sizeof(EndGameDataPacket);

            packet.result = 1; // win
            connections[0]->send(packet.toBuffer(), sizeof(EndGamePacket));
            packet.result = 2; // lost
            connections[1]->send(packet.toBuffer(), sizeof(EndGamePacket));

            return true;
        }

        // Player 2 win
        if (!characters[0]->getIsAlive() && characters[1]->getIsAlive())
        {
            EndGamePacket packet;
            packet.commandId = CommandId::C_END_GAME;
            packet.length = sizeof(EndGameDataPacket);

            packet.result = 1; // win
            connections[1]->send(packet.toBuffer(), sizeof(EndGamePacket));
            packet.result = 2; // lost
            connections[0]->send(packet.toBuffer(), sizeof(EndGamePacket));

            return true;
        }

        return false;
    }

    std::shared_ptr<ICharacter> _getCharacterByConnection(std::shared_ptr<TCPConnection> conn)
    {
        auto it = _players.find(conn);
        if (it != _players.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<ICharacter>> _getCharacters()
    {
        std::vector<std::shared_ptr<ICharacter>> result;
        for (const auto &player : _players)
        {
            result.push_back(player.second);
        }

        return result;
    }

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