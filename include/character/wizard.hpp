#pragma once

#include "core/game_world/resource/character/character.hpp"

class Wizard : public ICharacter
{
public:
    Wizard(float x, float y, bool isFlipped = false)
        : ICharacter(x, y, isFlipped)
    {
    }

    int setSpeed() override
    {
        return 240; // Wizard has a higher speed
    }

    int setArmor() override
    {
        return 2; // Wizard is nimble and has light armor
    }

    int setJumpHeight() override
    {
        return 40; // Wizard has a higher jump height
    }

    int setAtkCDamage() override
    {
        return 25; // Wizard's C attack deals more damage
    }

    int setAtkZDamage() override
    {
        return 15; // Wizard's Z attack deals moderate damage
    }
};