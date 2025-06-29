#pragma once

#include "core/game_world/resource/character/character.hpp"

class Fighter : public ICharacter
{
public:
    Fighter(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 300;
        _jumpHeight = 34;
        _atkCdamage = 25;
    }
};