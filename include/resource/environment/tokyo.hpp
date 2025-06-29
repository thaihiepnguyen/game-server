#pragma once
#include "core/game_world/resource/environment/environment.hpp"

class Tokyo : public IEnvironment
{
public:
    Tokyo() : IEnvironment(4.7f / 5.0f) {}
};
