#pragma once

#include "core/game_world/resource/character/character.hpp"

class Vampire : public ICharacter
{
public:
    Vampire(float x, float y)
        : ICharacter(x, y)
    {
        _speed = 300;
        _jumpHeight = 34;
        _atkCcooldown = 900;
        _atkZcooldown = 500;
    }
};