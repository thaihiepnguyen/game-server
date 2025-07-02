
#pragma once

class Kickable
{
public:
    virtual ~Kickable() = default;
    virtual float kickAway() = 0;
};
