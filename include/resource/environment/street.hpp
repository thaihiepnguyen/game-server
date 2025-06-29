#pragma once
#include "core/game_world/resource/environment/environment.hpp"

class Street : public IEnvironment
{
public:
    Street() : IEnvironment(4.7f / 5.0f) {}
};
