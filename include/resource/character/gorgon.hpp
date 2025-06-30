#pragma once

#include "core/game_world/resource/character/character.hpp"

class Gorgon : public ICharacter
{
public:
    Gorgon(float x, float y)
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
};