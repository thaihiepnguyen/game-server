#pragma once
#include "core/game_world/resource/environment/environment.hpp"

class Bridge : public IEnvironment
{
public:
    Bridge() : IEnvironment(4.3f / 5.0f) {}
};
