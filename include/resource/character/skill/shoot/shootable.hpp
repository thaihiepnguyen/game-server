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
    virtual ~Shootable()
    {
        for (auto &projectile : _projectiles)
        {
            if (projectile)
            {
                delete projectile;
            }
        }
    }

    virtual void shoot() = 0;

    const std::vector<IProjectile *> &getProjectiles() const
    {
        return _projectiles;
    }

    void updateProjectiles(float dt)
    {
        for (auto &projectile : _projectiles)
        {
            if (projectile)
            {
                Rect &rect = projectile->getRect();
                if (projectile->getIsFlipped())
                {
                    rect.setX(rect.getX() - projectile->getSpeed() * dt);
                }
                else
                {
                    rect.setX(rect.getX() + projectile->getSpeed() * dt);
                }

                // Check if the projectile is out of bounds
                if (rect.getX() < 0 || rect.getX() > WINDOW_WIDTH)
                {
                    _removeProjectile(projectile);
                }
            }
        }
    }

private:
    void _removeProjectile(IProjectile *projectile)
    {
        if (projectile)
            delete projectile;

        auto it = std::remove(_projectiles.begin(), _projectiles.end(), projectile);
        if (it != _projectiles.end())
        {
            _projectiles.erase(it, _projectiles.end());
        }
    }
};