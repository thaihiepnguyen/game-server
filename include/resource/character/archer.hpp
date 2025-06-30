#pragma once

#include "core/game_world/resource/character/character.hpp"
#include "resource/character/skill/shoot/shootable.hpp"
#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/arrow_projectile.hpp"

class Archer : public ICharacter, public Shootable
{
protected:
    Rect *getAttackCRect() const override
    {
        return nullptr;
    }

public:
    Archer(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 250;
        _armor = 2;
        _jumpHeight = 34;
        _atkCdamage = 25;
    }

    void shoot() override
    {
        float width = 160;
        float height = 30;
        float x = _isFlipped ? (_rect->getX() - width) : _rect->getRight();
        float y = _rect->getCenterY() - (height / 2);

        IProjectile *projectile = new ArrowProjectile(x, y, width, height, _isFlipped);
        _projectiles.push_back(projectile);
    }
};