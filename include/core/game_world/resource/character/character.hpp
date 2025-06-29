#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "core/util/const.hpp"
#include "core/util/time.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
class ICharacter
{
protected:
    int _hp = 100;
    int _maxHp = 100;
    int _armor = 3; // Armor value, if applicable
    Rect *_rect = nullptr;
    int _state = 0;      // 0 for idle, 1 for attack, etc.
    int _atkZdamage = 10; // Damage dealt by attack z
    int _atkXdamage = 15; // Damage dealt by attack x
    int _atkCdamage = 20; // Damage dealt by attack c

    bool _isFlipped = false;   // true for right, false for left
    int _atkZcooldown = 1000; // Cooldown for attack z
    int _atkXcooldown = 800; // Cooldown for attack x
    int _atkCcooldown = 1200; // Cooldown for attack c
    int _hitCoolDown = 500;  // Cooldown for hit

    float _velocityY = 0.0f; // Vertical velocity for jumping or falling
    int _speed = 250;       // Speed of the character, can be used for movement
    int _weight = 1;      // Weight of the character, can affect jumping and falling
    int _jumpHeight = 32;  // Height of the jump, if applicable

    // timers
    float _atkZtimer = 0.0f; // Timer for attack z cooldown
    float _atkXtimer = 0.0f; // Timer for attack x cooldown
    float _atkCtimer = 0.0f; // Timer for attack c cooldown
    float _hitTimer = 0.0f; // Timer for hit cooldown

    // flags
    bool _isMovingLeft = false;
    bool _isMovingRight = false;

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
    ICharacter(float x, float y)
        : _rect(new Rect(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT))
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
    void setIsMovingLeft(bool value);
    void setIsMovingRight(bool value);

    void attackZ();

    void attackX();

    void attackC();

    Rect *getAttackRect() const;

    float getAttackDamage();

    virtual ~ICharacter()
    {
        delete _rect;
    }

protected:
    Rect *getAttackZRect() const;

    Rect *getAttackXRect() const;

    Rect *getAttackCRect() const;

public:
    virtual void jump(float groundY);

    void lookAt(ICharacter *other);

    void stopMovement();

    void moveLeft(float dt);

    void moveRight(float dt);

    void defend();

    void undefend();

    void hit(int damage, int knockback = 15);
};