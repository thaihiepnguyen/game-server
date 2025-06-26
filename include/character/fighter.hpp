#pragma once

#include "core/resource/character/character.hpp"

class Fighter : public ICharacter
{
public:
    Fighter(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 300; // Fighter has a higher speed
    }

    int setArmor() override
    {
        return 3; // Fighter is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 34; // Fighter has a higher jump height
    }

    int setAtkCDamage() override
    {
        return 25; // Fighter's C attack deals more damage
    }
};