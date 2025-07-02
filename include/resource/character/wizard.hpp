#pragma once

#include "core/game_world/resource/character/character.hpp"
#include "resource/character/skill/shoot/shootable.hpp"
#include "resource/character/skill/shoot/fire_projectile.hpp"

class Wizard : public ICharacter, public Shootable
{
protected:
    Rect *getAttackCRect() const override;

public:
    Wizard(float x, float y);
    IProjectile *shoot() override;
};