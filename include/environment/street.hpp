#pragma once
#include "core/resource/environment/environment.hpp"

class Street : public IEnvironment
{
public:
    Street() : IEnvironment(4.7f / 5.0f) {}
};
