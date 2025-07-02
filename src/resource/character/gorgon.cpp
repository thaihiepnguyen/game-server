#include "resource/character/gorgon.hpp"

Gorgon::Gorgon(float x, float y)
    : ICharacter(x, y)
{
    _speed = 300;
    _armor = 4;
    _jumpHeight = 0;
    _atkCdamage = 25;
    _atkZcooldown = 1100;
    _atkXcooldown = 700;
    _atkCcooldown = 1000;
}