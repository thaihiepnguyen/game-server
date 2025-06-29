#include "core/game_world/resource/character/character.hpp"

void ICharacter::setIsMovingLeft(bool value) 
{
    if (value == true)
    {
        _isMovingLeft = true;
        _isMovingRight = false;
    } 
    else
    {
        _isMovingLeft = false;
    }
}
void ICharacter::setIsMovingRight(bool value)
{
    if (value == true)
    {
        _isMovingRight = true;
        _isMovingLeft = false;
    }
    else
    {
        _isMovingRight = false;
    }
}

void ICharacter::attackZ()
{
    if (Time::getCurrentTimeMs() - _atkZtimer >= _atkZcooldown)
    {
        _state = CharacterState::ATK_Z;
        _atkZtimer = Time::getCurrentTimeMs();
    }
}

void ICharacter::attackX()
{
    if (Time::getCurrentTimeMs() - _atkXtimer >= _atkXcooldown)
    {
        _state = CharacterState::ATK_X;
        _atkXtimer = Time::getCurrentTimeMs();
    }
}

void ICharacter::attackC()
{
    if (Time::getCurrentTimeMs() - _atkCtimer >= _atkXcooldown)
    {
        _state = CharacterState::ATK_C;
        _atkCtimer = Time::getCurrentTimeMs();
    }
}

Rect *ICharacter::getAttackRect() const
{
    if (_state == CharacterState::ATK_Z)
    {
        return getAttackZRect();
    }
    else if (_state == CharacterState::ATK_X)
    {
        return getAttackXRect();
    }
    else if (_state == CharacterState::ATK_C)
    {
        return getAttackCRect();
    }
    return nullptr;
}

float ICharacter::getAttackDamage()
{
    if (_state == CharacterState::ATK_Z)
    {
        return _atkZdamage;
    }
    else if (_state == CharacterState::ATK_X)
    {
        return _atkXdamage;
    }
    else if (_state == CharacterState::ATK_C)
    {
        return _atkCdamage;
    }
    return 0.0;
}

Rect *ICharacter::getAttackZRect() const
{
    return _getDefaultAttackRect();
}

Rect *ICharacter::getAttackXRect() const
{
    return _getDefaultAttackRect();
}

Rect *ICharacter::getAttackCRect() const
{
    return _getDefaultAttackRect();
}

void ICharacter::jump(float groundY)
{
    if (!getIsAlive())
    {
        return;
    }

    std::vector<int> statesBlockingMovement = _getStatesBlockingMovement();
    if (std::find(statesBlockingMovement.begin(), statesBlockingMovement.end(), _state) != statesBlockingMovement.end())
    {
        return; // Cannot jump if in a blocking state
    }

    if (getIsOnGround(groundY))
    {
        _state = CharacterState::JUMP;
        _velocityY = -_jumpHeight;
    }
}

void ICharacter::lookAt(ICharacter *other)
{
    if (!getIsAlive()) 
    {
        return;    
    }
    
    _isFlipped = (_rect->getCenterX() > other->getRect()->getCenterX());
}

void ICharacter::stopMovement()
{
    if (!getIsAlive()) 
    {
        return;    
    }

    setIsMovingLeft(false);
    setIsMovingRight(false);

    _state = CharacterState::IDLE;
}

void ICharacter::moveLeft(float dt)
{
    if (!getIsAlive())
    {
        return;
    }

    std::vector<int> statesBlockingMovement = _getStatesBlockingMovement();

    if (std::find(statesBlockingMovement.begin(), statesBlockingMovement.end(), _state) != statesBlockingMovement.end())
    {
        return; // Cannot move left if in a blocking state
    }

    float dx = -_speed * dt;

    float x = std::max(0.0f, std::min(_rect->getX() + dx, static_cast<float>(WINDOW_WIDTH - _rect->getWidth())));

    _rect->setX(x);
    if (_state != CharacterState::JUMP && _state != CharacterState::WALK)
    {
        _state = CharacterState::WALK;
    }
}

void ICharacter::moveRight(float dt)
{
    if (!getIsAlive())
    {
        return;
    }

    std::vector<int> statesBlockingMovement = _getStatesBlockingMovement();

    if (std::find(statesBlockingMovement.begin(), statesBlockingMovement.end(), _state) != statesBlockingMovement.end())
    {
        return; // Cannot move right if in a blocking state
    }

    float dx = _speed * dt;

    float x = std::max(0.0f, std::min(_rect->getX() + dx, static_cast<float>(WINDOW_WIDTH - _rect->getWidth())));

    _rect->setX(x);
    if (_state != CharacterState::JUMP && _state != CharacterState::WALK)
    {
        _state = CharacterState::WALK;
    }
}

void ICharacter::defend()
{
    if (!getIsAlive())
    {
        return;
    }
    _defTimer = Time::getCurrentTimeMs();
    _state = CharacterState::DEF;
}

void ICharacter::undefend()
{
    if (!getIsAlive())
    {
        return;
    }

    _state = CharacterState::IDLE;
}

void ICharacter::hit(int damage, int knockback)
{
    if (!getIsAlive() || _state == CharacterState::HIT)
    {
        return;
    }
    
    // Apply armor reduction
    int effectiveDamage = std::max(0, damage - _armor);
    _hp -= effectiveDamage;

    if (_hp <= 0)
    {
        _hp = 0;
        _state = CharacterState::DEATH;
    }
    else
    {
        _state = CharacterState::HIT;
    }

    // Apply knockback
    float knockbackDistance = knockback * (_isFlipped ? 1 : -1);
    float newX = std::max(0.0f, std::min(_rect->getX() + knockbackDistance, static_cast<float>(WINDOW_WIDTH - _rect->getWidth())));

    setX(newX);

    _hitTimer = Time::getCurrentTimeMs();
}