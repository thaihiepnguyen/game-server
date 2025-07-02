#pragma once

#include "core/game_world/resource/character/character.hpp"
#include "resource/character/skill/kick_away/kickable.hpp"

class Fighter : public ICharacter, public Kickable
{
public:
    Fighter(float x, float y);

    float kickAway() override;
};