#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "core/util/const.hpp"
#include "core/util/time.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
class ICharacter
{
private:
    int _hp;
    int _maxHp;
    int _armor; // Armor value, if applicable
    Rect *_rect;
    int _state;      // 0 for idle, 1 for attack, etc.
    int _atkZdamage; // Damage dealt by attack z
    int _atkXdamage; // Damage dealt by attack x
    int _atkCdamage; // Damage dealt by attack c

    bool _isFlipped;   // true for right, false for left
    int _atkZcooldown; // Cooldown for attack z
    int _atkXcooldown; // Cooldown for attack x
    int _atkCcooldown; // Cooldown for attack c
    int _hitCoolDown;  // Cooldown for hit

    float _velocityY; // Vertical velocity for jumping or falling
    int _speed;       // Speed of the character, can be used for movement
    int _weight;      // Weight of the character, can affect jumping and falling
    int _jumpHeight;  // Height of the jump, if applicable

    // timers
    float _atkZtimer; // Timer for attack z cooldown
    float _atkXtimer; // Timer for attack x cooldown
    float _atkCtimer; // Timer for attack c cooldown
    float _hitTimer; // Timer for hit cooldown

    // flags
    bool _isMovingLeft;
    bool _isMovingRight;

    Rect *_getDefaultAttackRect() const
    {
        float width = _rect->getWidth() * 2.0f;
        float height = _rect->getHeight() * 0.3f;
        float x = _isFlipped ? _rect->getCenterX() - width : _rect->getCenterX();
        float y = _rect->getY() + _rect->getHeight() * 0.5f - height * 0.5f;
        return new Rect(x, y, width, height);
    }

    std::vector<int> _getStatesBlockingMovement() const
    {
        return 
        {
            CharacterState::DEF,
            CharacterState::ATK_Z,
            CharacterState::ATK_X,
            CharacterState::ATK_C,
            CharacterState::HIT,
            CharacterState::DEATH,
        };
    }

public:
    ICharacter(float x, float y, bool isFlipped = false)
        : _hp(100), _maxHp(100), _rect(new Rect(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT)), _state(0), _atkZdamage(setAtkZDamage()), _atkXdamage(setAtkXDamage()), _atkCdamage(setAtkCDamage()), _isFlipped(isFlipped), _atkZtimer(0.0f), _atkXtimer(0.0f), _atkCtimer(0.0f), _velocityY(0.0f), _speed(setSpeed()), _weight(setWeight()), _jumpHeight(setJumpHeight()), _armor(setArmor()), _atkZcooldown(setAtkZCooldown()), _atkXcooldown(setAtkXCooldown()), _atkCcooldown(setAtkCCooldown()), _isMovingLeft(false), _isMovingRight(false), _hitCoolDown(setHitCooldown()),
        _hitTimer(0.0f)
    {
    }

    // Getters
    int getHp() const { return _hp; }
    int getMaxHp() const { return _maxHp; }
    int getArmor() const { return _armor; }
    int getState() const { return _state; }
    bool getIsFlipped() const { return _isFlipped; }
    bool getIsAlive() const { return _hp > 0; }
    bool getIsDefending() const { return _state == CharacterState::DEF; }
    Rect* getRect() const { return _rect; }
    float getVelocityY() const { return _velocityY; }
    float getX() const { return _rect->getX(); }
    float getY() const { return _rect->getY(); }
    bool getIsMovingLeft() const { return _isMovingLeft; }
    bool getIsMovingRight() const { return _isMovingRight; }
    bool getIsOnGround(float groundY) const { return _rect->getBottom() == groundY; }

    float getAtkXCooldown() const { return _atkXcooldown; }
    float getAtkXTimer() const { return _atkXtimer; }
    float getAtkZCooldown() const { return _atkZcooldown; }
    float getAtkZTimer() const { return _atkZtimer; }
    float getAtkCCooldown() const { return _atkCcooldown; }
    float getAtkCTimer() const { return _atkCtimer; }
    float getHitCoolDown() const { return _hitCoolDown; }
    float getHitTimer() const { return _hitTimer; }


    // Setters
    void setHp(int hp) { _hp = hp; }
    void setState(int state) { _state = state; }
    void setFlipped(bool isFlipped) { _isFlipped = isFlipped; }
    void setX(float x) { _rect->setX(x); }
    void setY(float y) { _rect->setY(y); }
    void setWidth(float width) { _rect->setWidth(width); }
    void setHeight(float height) { _rect->setHeight(height); }
    void setVelocityY(float velocityY) { _velocityY = velocityY; }
    void setIsMovingLeft(bool value) 
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
    void setIsMovingRight(bool value)
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

    void attackZ()
    {
        if (Time::getCurrentTimeMs() - _atkZtimer >= _atkZcooldown)
        {
            _state = CharacterState::ATK_Z;
            _atkZtimer = Time::getCurrentTimeMs();
        }
    }

    void attackX()
    {
        if (Time::getCurrentTimeMs() - _atkXtimer >= _atkXcooldown)
        {
            _state = CharacterState::ATK_X;
            _atkXtimer = Time::getCurrentTimeMs();
        }
    }

    void attackC()
    {
        if (Time::getCurrentTimeMs() - _atkCtimer >= _atkXcooldown)
        {
            _state = CharacterState::ATK_C;
            _atkCtimer = Time::getCurrentTimeMs();
        }
    }

    Rect *getAttackRect() const
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

    float getAttackDamage()
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

    virtual ~ICharacter()
    {
        delete _rect;
    }
    
    virtual int setSpeed()
    {
        return 200;
    }

    virtual int setWeight()
    {
        return 1;
    }

    virtual int setJumpHeight()
    {
        return 32;
    }

    virtual int setArmor()
    {
        return 3;
    }

    virtual int setAtkZDamage()
    {
        return 10;
    }

    virtual int setAtkXDamage()
    {
        return 15;
    }

    virtual int setAtkCDamage()
    {
        return 20;
    }

    virtual int setAtkZCooldown()
    {
        return 1000; // 1 second cooldown
    }

    virtual int setAtkXCooldown()
    {
        return 800; // 0.8 second cooldown
    }

    virtual int setAtkCCooldown()
    {
        return 1200; // 1.2 second cooldown
    }

    virtual int setHitCooldown()
    {
        return 500; // 0.5 second cooldown
    }

protected:
    Rect *getAttackZRect() const
    {
        return _getDefaultAttackRect();
    }

    Rect *getAttackXRect() const
    {
        return _getDefaultAttackRect();
    }

    Rect *getAttackCRect() const
    {
        return _getDefaultAttackRect();
    }

public:
    virtual void jump(float groundY)
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

    void lookAt(ICharacter *other)
    {
        if (!getIsAlive()) 
        {
            return;    
        }
        
        _isFlipped = (_rect->getCenterX() > other->getRect()->getCenterX());
    }

    void stopMovement()
    {
        if (!getIsAlive()) 
        {
            return;    
        }

        setIsMovingLeft(false);
        setIsMovingRight(false);

        _state = CharacterState::IDLE;
    }

    void moveLeft(float dt)
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

    void moveRight(float dt)
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

    void defend()
    {
        if (!getIsAlive())
        {
            return;
        }

        _state = CharacterState::DEF;
    }

    void undefend()
    {
        if (!getIsAlive())
        {
            return;
        }

        _state = CharacterState::IDLE;
    }

    void hit(int damage, int knockback = 15)
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
};