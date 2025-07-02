#include "resource/character/fighter.hpp"

Fighter::Fighter(float x, float y)
    : ICharacter(x, y)
{
    _speed = 300;
    _jumpHeight = 34;
    _atkCdamage = 25;
    _atkZcooldown = 600;
    _atkXcooldown = 300;
    _atkCcooldown = 400;
}

float Fighter::kickAway()
{
    float knockBack = 200.0f; // Knockback distance
    return knockBack;
}