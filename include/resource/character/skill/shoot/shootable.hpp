#pragma once

#include "core/game_world/resource/rectangle/rectangle.hpp"
#include "resource/character/skill/shoot/projectile.hpp"
#include <vector>
#include "core/util/const.hpp"

class Shootable
{
protected:
    std::vector<IProjectile *> _projectiles;

public:
    virtual ~Shootable();

    virtual IProjectile *shoot() = 0;

    const std::vector<IProjectile *> &getProjectiles() const;

    void updateProjectiles(float dt);

private:
    void _removeProjectile(IProjectile *projectile);
};