#pragma once

#include <chrono>

class Time
{
public:
    static long long getCurrentTimeMs()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }
};
