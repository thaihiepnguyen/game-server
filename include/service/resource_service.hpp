#pragma once

#include "core/service/service.hpp"
#include <unordered_map>
#include <string>
#include "core/utils/const.hpp"

class ResourceService : public IService
{
public:
    ResourceService()
    {
    }

    void inject(std::shared_ptr<Provider> provider) override
    {
        // This method can be used to inject dependencies if needed
    }

    int getRandomCharacterId()
    {
        return (rand() % CHARACTER_LENGTH) + 1;
    }

    int getRandomBackgroundId()
    {
        return (rand() % BACKGROUND_LENGTH) + 1;
    }
};