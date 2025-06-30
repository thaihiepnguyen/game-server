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
    }
};