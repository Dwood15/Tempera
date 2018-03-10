#pragma once

#include <stdint.h>

typedef enum ServerType : uint16_t {
    SERVER_NONE = 0,
    SERVER_DEDICATED = 1,
    SERVER_LOCAL = 2
} ServerType;
ServerType server_type();

typedef enum Gametype : uint16_t  {
    GAMETYPE_CTF = 1,
    GAMETYPE_SLAYER = 2,
    GAMETYPE_ODDBALL = 3,
    GAMETYPE_KING = 4,
    GAMETYPE_RACE = 5
} Gametype;
Gametype gametype();
