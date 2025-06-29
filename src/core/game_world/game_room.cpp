#include "core/game_world/game_room.hpp"

std::condition_variable GameRoom::cv;
std::mutex GameRoom::cv_mtx;


void GameRoom::_startGameLoop()
{
    while (_gameRunning)
    {
        _update(_tick);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(_tick * 1000)));
    }
}

void GameRoom::_update(float dt)
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

void GameRoom::_processQueuedPackets(float dt)
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

void GameRoom::_updateBattle(float dt)
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

void GameRoom::_updateCharacterStates(float dt)
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

void GameRoom::_updateCollisions()
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

void GameRoom::_broadcastUpdate()
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

void GameRoom::_updateInteractionBetween(std::shared_ptr<ICharacter> character, std::shared_ptr<ICharacter> opponent)
{
    auto attackRect = character->getAttackRect();

    if (attackRect != nullptr && attackRect->collidesWith(opponent->getRect()))
    {
        // Only accept defense with first 100ms
        if (opponent->getIsDefending() && (Time::getCurrentTimeMs() - opponent->getDefTimer() <= 100))
        {
            return;
        }
        std::shared_ptr<Rect> intersection(attackRect->clip(opponent->getRect()));
        if (intersection != nullptr)
        {
            float damgeRatio = character->getAttackDamage() * (intersection->getWidth() / opponent->getRect()->getWidth());
            opponent->hit((int)damgeRatio);
        }

    }
}

bool GameRoom::_isEndGame()
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

std::shared_ptr<ICharacter> GameRoom::_getCharacterByConnection(std::shared_ptr<TCPConnection> conn)
{
    auto it = _players.find(conn);
    if (it != _players.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<ICharacter>> GameRoom::_getCharacters()
{
    std::vector<std::shared_ptr<ICharacter>> result;
    for (const auto &player : _players)
    {
        result.push_back(player.second);
    }

    return result;
}