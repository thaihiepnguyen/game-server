#pragma once

#include <cmath>
#include <unordered_map>

enum CommandId
{
    WAIT_FOR_MATCH = 1,
    BROADCAST = 2,
    OPPONENT_OUT = 3
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

const std::unordered_map<std::string, int> CHARACTER_STATES = {
    {"idle", 0},
    {"walk", 1},
    {"jump", 2},
    {"def", 3},
    {"hit", 4},
    {"atk_z", 5},
    {"atk_x", 6},
    {"atk_c", 7},
    {"death", 8}};

const enum QueuedPacketType {
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    STOP_MOVE = 3,
    ATTACK_Z = 4,
    ATTACK_X = 5,
    ATTACK_C = 6,
    JUMP = 7,
    DEFEND = 8,
    UNDEFEND = 9,
    HIT = 10,
    DEATH = 11,
};