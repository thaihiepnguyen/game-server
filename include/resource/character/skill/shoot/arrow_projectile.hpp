#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/projectile.hpp"

class ArrowProjectile : public IProjectile
{
public:
    ArrowProjectile(float x, float y, float width, float height, float damage, bool isFlipped = false);
};
