#include "resource/character/wizard.hpp"

Wizard::Wizard(float x, float y)
    : ICharacter(x, y)
{
    _speed = 240;
    _armor = 2;
    _jumpHeight = 32;
    _atkCdamage = 25;
    _atkZdamage = 15;
    _atkZcooldown = 350;
    _atkXcooldown = 450;
    _atkCcooldown = 1600;
}

IProjectile *Wizard::shoot()
{
    if (_state == CharacterState::ATK_C)
    {
        float width = 90;
        float height = 30;
        float x = _isFlipped ? (_rect->getX() - width) : _rect->getRight();
        float y = _rect->getCenterY() - (height / 2);

        IProjectile *projectile = new FireProjectile(x, y, width, height, _atkCdamage, _isFlipped);
        _projectiles.push_back(projectile);
        return projectile;
    }

    return nullptr;
}

Rect *Wizard::getAttackCRect() const
{
    return nullptr;
}