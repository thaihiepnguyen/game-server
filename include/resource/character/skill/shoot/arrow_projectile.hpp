#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/projectile.hpp"

class ArrowProjectile : public IProjectile
{
public:
    ArrowProjectile(float x, float y, float width, float height, bool isFlipped = false)
    {
        _rect = new Rect(x, y, width, height);
        _speed = 800;
        _isFlipped = isFlipped; // true for right, false for left
    }
};
