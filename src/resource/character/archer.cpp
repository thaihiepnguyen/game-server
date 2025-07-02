#include "resource/character/archer.hpp"

Rect *Archer::getAttackCRect() const
{
    return nullptr;
}

Archer::Archer(float x, float y)
    : ICharacter(x, y)
{
    _speed = 250;
    _armor = 2;
    _jumpHeight = 34;
    _atkCdamage = 25;
    _atkZcooldown = 500;
    _atkXcooldown = 250;
    _atkCcooldown = 1000;
}

IProjectile *Archer::shoot()
{
    if (_state == CharacterState::ATK_C)
    {
        float width = 160;
        float height = 30;
        float x = _isFlipped ? (_rect->getX() - width) : _rect->getRight();
        float y = _rect->getCenterY() - (height);

        IProjectile *projectile = new ArrowProjectile(x, y, width, height, _atkCdamage, _isFlipped);
        _projectiles.push_back(projectile);
        return projectile;
    }

    return nullptr;
}