#pragma once

#include "core/service/service.hpp"
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include "core/util/const.hpp"
#include "core/game_world/resource/character/character.hpp"
#include "core/game_world/resource/environment/environment.hpp"

class ResourceService : public IService
{
public:
    ResourceService()
    {
        srand(time(0));
    }

    void inject(std::shared_ptr<Provider> provider) override
    {
        // This method can be used to inject dependencies if needed
    }

    int getRandomCharacterId();

    int getRandomBackgroundId();

    ICharacter *createCharacter(int characterId, float x, float y);
    IEnvironment *createEnvironment(int backgroundId);
};