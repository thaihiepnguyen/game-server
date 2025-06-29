#include "service/resource_service.hpp"
#include "core/game_world/resource/character/character.hpp"
#include "core/game_world/resource/environment/environment.hpp"
#include "resource/character/archer.hpp"
#include "resource/character/fighter.hpp"
#include "resource/character/gorgon.hpp"
#include "resource/character/vampire.hpp"
#include "resource/character/wizard.hpp"
#include "resource/character/tengu.hpp"
#include "core/util/const.hpp"
#include "resource/environment/bridge.hpp"
#include "resource/environment/countryside.hpp"
#include "resource/environment/street.hpp"
#include "resource/environment/temple.hpp"
#include "resource/environment/tokyo.hpp"

#include <stdexcept>

int ResourceService::getRandomCharacterId()
{
    return rand() % CHARACTER_LENGTH + 1;
}

int ResourceService::getRandomBackgroundId()
{
    return rand() % BACKGROUND_LENGTH + 1;
}

ICharacter *ResourceService::createCharacter(int characterId, float x, float y, bool isFlipped)
{
    switch (characterId)
    {
    case CharacterId::ARCHER:
        return new Archer(x, y, isFlipped);
    case CharacterId::FIGHTER:
        return new Fighter(x, y, isFlipped);
    case CharacterId::GORGON:
        return new Gorgon(x, y, isFlipped);
    case CharacterId::VAMPIRE:
        return new Vampire(x, y, isFlipped);
    case CharacterId::WIZARD:
        return new Wizard(x, y, isFlipped);
    case CharacterId::TENGU:
        return new Tengu(x, y, isFlipped);
    default:
        throw std::invalid_argument("Invalid character ID");
    }
}

IEnvironment *ResourceService::createEnvironment(int backgroundId)
{
    switch (backgroundId)
    {
    case BackgroundId::BRIDGE:
        return new Bridge();
    case BackgroundId::COUNTRY_SIDE:
        return new Countryside();
    case BackgroundId::STREET:
        return new Street();
    case BackgroundId::TEMPLE:
        return new Temple();
    case BackgroundId::TOKYO:
        return new Tokyo();
    default:
        throw std::invalid_argument("Invalid background ID");
    }
}