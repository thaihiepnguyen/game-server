#include "service/resource_service.hpp"
#include "core/resource/character/character.hpp"
#include "core/resource/environment/environment.hpp"
#include "character/archer.hpp"
#include "character/fighter.hpp"
#include "character/gorgon.hpp"
#include "character/vampire.hpp"
#include "character/wizard.hpp"
#include "character/tengu.hpp"
#include "core/util/const.hpp"
#include "environment/bridge.hpp"
#include "environment/countryside.hpp"
#include "environment/street.hpp"
#include "environment/temple.hpp"
#include "environment/tokyo.hpp"

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