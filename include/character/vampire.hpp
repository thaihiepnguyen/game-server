#pragma once

#include "core/resource/character/character.hpp"

class Vampire : public ICharacter
{
public:
    Vampire(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 300; // Vampire has a higher speed
    }

    int setArmor() override
    {
        return 3; // Vampire is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 34; // Vampire has a higher jump height
    }

    int setAtkCCooldown() override
    {
        return 900; // 0.9 second cooldown
    }
};