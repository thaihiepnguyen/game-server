#include "resource/character/skill/shoot/fire_projectile.hpp"

FireProjectile::FireProjectile(float x, float y, float width, float height, float damage, bool isFlipped)
{
    _rect = new Rect(x, y, width, height);
    _speed = 1000;
    _isFlipped = isFlipped; // true for right, false for left
    _damage = damage;
}