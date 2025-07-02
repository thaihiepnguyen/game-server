#include "resource/character/tengu.hpp"

Tengu::Tengu(float x, float y)
    : ICharacter(x, y)
{
    _speed = 320;
    _jumpHeight = 40;
    _atkCdamage = 20;
    _atkZcooldown = 500;
    _atkXcooldown = 250;
    _atkCcooldown = 350;
}
