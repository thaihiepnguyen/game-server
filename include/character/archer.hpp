#pragma once

#include "core/resource/character/character.hpp"

class Archer : public ICharacter
{
public:
    Archer(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 250; // Archer has a higher speed
    }

    int setArmor() override
    {
        return 2; // Archer is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 34; // Archer has a higher jump height
    }

    int setAtkCDamage() override
    {
        return 25; // Archer's C attack deals more damage
    }
};