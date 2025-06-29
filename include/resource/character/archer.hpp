#pragma once

#include "core/game_world/resource/character/character.hpp"

class Archer : public ICharacter
{
public:
    Archer(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 250;
        _armor = 2;
        _jumpHeight = 34;
        _atkCdamage = 25;
    }
};