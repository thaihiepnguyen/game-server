#pragma once

#include "core/resource/rectangle/rectangle.hpp"
#include "core/util/const.hpp"
#include "core/util/time.hpp"
#include <cmath>
#include <algorithm>
class ICharacter
{
private:
    int _hp;
    int _maxHp;
    int _armor; // Armor value, if applicable
    Rect _rect;
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
    bool _is_moving_left;
    bool _is_moving_right;

    Rect *_getDefaultAttackBox() const
    {
        float width = _rect.getWidth() * 1.2f;
        float height = _rect.getHeight() * 0.3f;
        float x = _isFlipped ? _rect.getCenterX() - width : _rect.getCenterX();
        float y = _rect.getY() + _rect.getHeight() * 0.5f - height * 0.5f;
        return new Rect(x, y, width, height);
    }

    std::vector<int> _getStatesBlockingMovement() const
    {
        return {CHARACTER_STATES.at("def"), CHARACTER_STATES.at("atk_z"), CHARACTER_STATES.at("atk_x"), CHARACTER_STATES.at("atk_c"), CHARACTER_STATES.at("hit"), CHARACTER_STATES.at("death"), CHARACTER_STATES.at("hit")};
    }

public:
    ICharacter(float x, float y, bool isFlipped = false)
        : _hp(100), _maxHp(100), _rect(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT), _state(0), _atkZdamage(setAtkZDamage()), _atkXdamage(setAtkXDamage()), _atkCdamage(setAtkCDamage()), _isFlipped(isFlipped), _atkZtimer(0.0f), _atkXtimer(0.0f), _atkCtimer(0.0f), _velocityY(0.0f), _speed(setSpeed()), _weight(setWeight()), _jumpHeight(setJumpHeight()), _armor(setArmor()), _atkZcooldown(setAtkZCooldown()), _atkXcooldown(setAtkXCooldown()), _atkCcooldown(setAtkCCooldown()), _is_moving_left(false), _is_moving_right(false), _hitCoolDown(setHitCooldown()),
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
    bool getIsDefending() const { return _state == CHARACTER_STATES.at("def"); }
    Rect getRect() const { return _rect; }
    float getVelocityY() const { return _velocityY; }
    float getX() const { return _rect.getX(); }
    float getY() const { return _rect.getY(); }
    bool getIsMovingLeft() const { return _is_moving_left; }
    bool getIsMovingRight() const { return _is_moving_right; }
    bool getIsOnGround() const { return _velocityY == 0.0f; }

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
    void setX(float x) { _rect.setX(x); }
    void setY(float y) { _rect.setY(y); }
    void setWidth(float width) { _rect.setWidth(width); }
    void setHeight(float height) { _rect.setHeight(height); }
    void setVelocityY(float velocityY) { _velocityY = velocityY; }
    void setIsMovingLeft(bool value) 
    {
        if (value == true)
        {
            _is_moving_left = true;
            _is_moving_right = false;
        } 
        else
        {
            _is_moving_left = false;
        }
    }
    void setIsMovingRight(bool value)
    {
        if (value == true)
        {
            _is_moving_right = true;
            _is_moving_left = false;
        }
        else
        {
            _is_moving_right = false;
        }
    }

    void attackZ()
    {
        if (Time::getCurrentTimeMs() - _atkZtimer >= _atkZcooldown)
        {
            _state = CHARACTER_STATES.at("atk_z");
            _atkZtimer = Time::getCurrentTimeMs();
        }
    }

    void attackX()
    {
        if (Time::getCurrentTimeMs() - _atkXtimer >= _atkXcooldown)
        {
            _state = CHARACTER_STATES.at("atk_x");
            _atkXtimer = Time::getCurrentTimeMs();
        }
    }

    void attackC()
    {
        if (Time::getCurrentTimeMs() - _atkCtimer >= _atkXcooldown)
        {
            _state = CHARACTER_STATES.at("atk_c");
            _atkCtimer = Time::getCurrentTimeMs();
        }
    }

    Rect *getAttackBox() const
    {
        if (_state == CHARACTER_STATES.at("atk_z"))
        {
            return getAttackZBox();
        }
        else if (_state == CHARACTER_STATES.at("atk_x"))
        {
            return getAttackXBox();
        }
        else if (_state == CHARACTER_STATES.at("atk_c"))
        {
            return getAttackCBox();
        }
        return nullptr;
    }

    virtual ~ICharacter() = default;
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
    Rect *getAttackZBox() const
    {
        return _getDefaultAttackBox();
    }

    Rect *getAttackXBox() const
    {
        return _getDefaultAttackBox();
    }

    Rect *getAttackCBox() const
    {
        return _getDefaultAttackBox();
    }

public:
    void jump()
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

        _state = CHARACTER_STATES.at("jump");

        if (getIsOnGround())
        {
            _velocityY = -_jumpHeight;
        }
    }

    void lookAt(ICharacter *other)
    {
        if (!getIsAlive()) {
            return;    
        }
        
        _isFlipped = (_rect.getCenterX() > other->getRect().getCenterX());
    }

    void stopMovement()
    {
        setIsMovingLeft(false);
        setIsMovingRight(false);

        _state = CHARACTER_STATES.at("idle");
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

        float x = std::max(0.0f, std::min(_rect.getX() + dx, static_cast<float>(WINDOW_WIDTH - _rect.getWidth())));

        _rect.setX(x);
        if (_state != CHARACTER_STATES.at("jump") && _state != CHARACTER_STATES.at("walk"))
        {
            _state = CHARACTER_STATES.at("walk");
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

        float x = std::max(0.0f, std::min(_rect.getX() + dx, static_cast<float>(WINDOW_WIDTH - _rect.getWidth())));

        _rect.setX(x);
        if (_state != CHARACTER_STATES.at("jump") && _state != CHARACTER_STATES.at("walk"))
        {
            _state = CHARACTER_STATES.at("walk");
        }
    }

    void defend()
    {
        if (!getIsAlive())
        {
            return;
        }
        _state = CHARACTER_STATES.at("def");
    }

    void undefend()
    {
        if (!getIsAlive())
        {
            return;
        }
        _state = CHARACTER_STATES.at("idle");
    }

    void hit(int damage, int knockback = 15)
    {
        if (!getIsAlive())
        {
            return;
        }

        // Apply armor reduction
        int effectiveDamage = std::max(0, damage - _armor);
        _hp -= effectiveDamage;

        if (_hp <= 0)
        {
            _hp = 0;
            _state = CHARACTER_STATES.at("death");
        }
        else
        {
            _state = CHARACTER_STATES.at("hit");
        }

        // Apply knockback
        float knockbackDistance = knockback * (_isFlipped ? -1 : 1);
        float newX = std::max(0.0f, std::min(_rect.getX() + knockbackDistance, static_cast<float>(WINDOW_WIDTH - _rect.getWidth())));

        setX(newX);

        _hitTimer = Time::getCurrentTimeMs();
    }
};