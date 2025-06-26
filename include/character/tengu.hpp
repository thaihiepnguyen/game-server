#pragma once

#include "core/resource/character/character.hpp"

class Tengu : public ICharacter
{
public:
    Tengu(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 230; // Tengu has a higher speed
    }

    int setArmor() override
    {
        return 4; // Tengu is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 40; // Tengu has a higher jump height
    }

    int setAtkCDamage() override
    {
        return 25; // Tengu's C attack deals more damage
    }
};