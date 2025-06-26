#pragma once
#include "core/resource/environment/environment.hpp"

class Countryside : public IEnvironment
{
public:
    Countryside() : IEnvironment(4.7f / 5.0f) {}
};
