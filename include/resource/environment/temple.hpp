#pragma once
#include "core/game_world/resource/environment/environment.hpp"

class Temple : public IEnvironment
{
public:
    Temple() : IEnvironment(4.7f / 5.0f) {}
};
