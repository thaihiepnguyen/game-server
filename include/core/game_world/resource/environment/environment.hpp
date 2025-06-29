#pragma once

class IEnvironment
{
private:
    float _groundYRatio;

public:
    IEnvironment(float groundYRatio) : _groundYRatio(groundYRatio) {}

    float getGroundYRatio() const { return _groundYRatio; }
};