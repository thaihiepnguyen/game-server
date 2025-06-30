#pragma once

#include "core/game_world/resource/character/character.hpp"

class Tengu : public ICharacter
{
public:
    Tengu(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 230;
        _armor = 4;
        _jumpHeight = 38;
        _atkCdamage = 25;
        _atkZcooldown = 150;
        _atkXcooldown = 300;
        _atkCcooldown = 600;
    }
};