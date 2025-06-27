#pragma once

#include <cmath>
#include <unordered_map>

enum CommandId
{
    WAIT_FOR_MATCH = 1,
    BROADCAST = 2,
    OPPONENT_OUT = 3,
    MOVE = 3,
};

enum CharacterId
{
    ARCHER = 1,
    FIGHTER = 2,
    GORGON = 3,
    VAMPIRE = 4,
    WIZARD = 5,
    TENGU = 6,
};

enum BackgroundId
{
    BRIDGE = 1,
    COUNTRY_SIDE = 2,
    STREET = 3,
    TEMPLE = 4,
    TOKYO = 5,
};

const int CHARACTER_LENGTH = 6;
const int BACKGROUND_LENGTH = 5;

const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 150;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

const float GRAVITY = 2.0f; // Gravity acceleration

enum CharacterState
{
    IDLE = 0,
    WALK = 1,
    JUMP = 2,
    DEF = 3,
    HIT = 4,
    ATK_Z = 5,
    ATK_X = 6,
    ATK_C = 7,
    DEATH = 8
};

enum QueuedPacketType {
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    STOP_MOVE = 3,
    ATTACK_Z = 4,
    ATTACK_X = 5,
    ATTACK_C = 6,
    PAC_JUMP = 7,
    DEFEND = 8,
    UNDEFEND = 9,
};