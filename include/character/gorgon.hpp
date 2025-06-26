#pragma once

#include "core/resource/character/character.hpp"

class Gorgon : public ICharacter
{
public:
    Gorgon(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 300; // Gorgon has a higher speed
    }

    int setArmor() override
    {
        return 3; // Gorgon is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 0; // Gorgon has no jump height
    }

    int setAtkCDamage() override
    {
        return 25; // Gorgon's C attack deals more damage
    }
};