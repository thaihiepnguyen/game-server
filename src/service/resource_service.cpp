#include "service/resource_service.hpp"


int ResourceService::getRandomCharacterId()
{
    return rand() % CHARACTER_LENGTH + 1;
}

int ResourceService::getRandomBackgroundId()
{
    return rand() % BACKGROUND_LENGTH + 1;
}