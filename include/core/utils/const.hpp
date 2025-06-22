#pragma once

enum CommandId {
    WAIT_FOR_MATCH = 1,
    BROADCAST = 2
};

enum CharacterId {
    ARCHER = 1,
    FIGHTER = 2,
    GORGON = 3,
    VAMPIRE = 4,
    WIZARD = 5,
    TENGU = 6,
};

enum BackgroundId {
    BRIDGE = 1,
    COUNTRY_SIDE = 2,
    STREET = 3,
    TEMPLE = 4,
    TOKYO = 5,
};

const int CHARACTER_LENGTH = 6;
const int BACKGROUND_LENGTH = 5;