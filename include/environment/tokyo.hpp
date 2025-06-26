#pragma once
#include "core/resource/environment/environment.hpp"

class Tokyo : public IEnvironment
{
public:
    Tokyo() : IEnvironment(4.3f / 5.0f) {}
};
