#include "resource/character/skill/shoot/arrow_projectile.hpp"

ArrowProjectile::ArrowProjectile(float x, float y, float width, float height, float damage, bool isFlipped)
{
    _rect = new Rect(x, y, width, height);
    _speed = 800;
    _isFlipped = isFlipped; // true for right, false for left
    _damage = damage;
}