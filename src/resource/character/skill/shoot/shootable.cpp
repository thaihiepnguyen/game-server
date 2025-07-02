#include "resource/character/skill/shoot/shootable.hpp"

Shootable::~Shootable()
{
    for (auto &projectile : _projectiles)
    {
        if (projectile)
        {
            delete projectile;
        }
    }
}

void Shootable::_removeProjectile(IProjectile *projectile)
{
    if (projectile)
        delete projectile;

    auto it = std::remove(_projectiles.begin(), _projectiles.end(), projectile);
    if (it != _projectiles.end())
    {
        _projectiles.erase(it, _projectiles.end());
    }
}

void Shootable::updateProjectiles(float dt)
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

const std::vector<IProjectile *> &Shootable::getProjectiles() const
{
    return _projectiles;
}