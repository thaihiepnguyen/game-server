#pragma once

#include "core/game_world/resource/character/character.hpp"
#include "resource/character/skill/shoot/shootable.hpp"
#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/arrow_projectile.hpp"

class Archer : public ICharacter, public Shootable
{
protected:
    Rect *getAttackCRect() const override;

public:
    Archer(float x, float y);

    IProjectile *shoot() override;
};