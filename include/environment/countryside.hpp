#pragma once
#include "core/game_world/resource/environment/environment.hpp"

class Countryside : public IEnvironment
{
public:
    Countryside() : IEnvironment(4.7f / 5.0f) {}
};
