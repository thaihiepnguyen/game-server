#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"

class IProjectile
{
protected:
    float _speed;
    Rect *_rect;
    bool _isFlipped = false; // true for right, false for left

public:
    IProjectile() {}

    Rect &getRect() const { return *_rect; }
    bool getIsFlipped() const { return _isFlipped; }
    float getSpeed() const { return _speed; }

    virtual ~IProjectile()
    {
        delete _rect;
    }
};