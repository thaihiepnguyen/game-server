#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/projectile.hpp"

class FireProjectile : public IProjectile
{
public:
    FireProjectile(float x, float y, float width, float height, float damage, bool isFlipped = false);
};
