#pragma once

#include "core/game_world/resource/character/character.hpp"

class Wizard : public ICharacter
{
public:
    Wizard(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 240;
        _armor = 2;
        _jumpHeight = 38;
        _atkCdamage = 25;
        _atkZdamage = 15;
        _atkZcooldown = 350;
        _atkXcooldown = 450;
        _atkCcooldown = 1600;
    }
};