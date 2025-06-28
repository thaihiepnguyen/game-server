#pragma once
#include "core/resource/environment/environment.hpp"

class Tokyo : public IEnvironment
{
public:
    Tokyo() : IEnvironment(4.7f / 5.0f) {}
};
