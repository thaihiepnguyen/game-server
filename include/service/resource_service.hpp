#pragma once

#include "core/service/service.hpp"
#include <unordered_map>
#include <string>
#include <random>
#include <ctime>
#include "core/utils/const.hpp"

class ResourceService : public IService
{
private:
    std::mt19937 _rng;

public:
    ResourceService() : _rng(std::time(nullptr))
    {
    }

    void inject(std::shared_ptr<Provider> provider) override
    {
        // This method can be used to inject dependencies if needed
    }

    int getRandomCharacterId()
    {
        std::uniform_int_distribution<int> dist(1, CHARACTER_LENGTH);
        return dist(_rng);
    }

    int getRandomBackgroundId()
    {
        std::uniform_int_distribution<int> dist(1, BACKGROUND_LENGTH);
        return dist(_rng);
    }
};