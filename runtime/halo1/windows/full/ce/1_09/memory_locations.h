#pragma once

#include <cstdint>
#include <versions.h>

//I recognize this isn't an ideal constexpr setup.
constexpr engines::major ENGINE_TARGET = engines::major::CE;
constexpr engines::with_minor GAME_MINOR = engines::with_minor::halo_1_10;


//HaloForge
ADDR CORE0 = 0x815900;
ADDR CORE1 = 0x7FB6F8;
ADDR CORE2 = 0x7FBE70;
ADDR CORE4 = 0x81B800;
ADDR CORE5 = 0x81B894;
ADDR CORE6 = 0x653BE4;

//TODO: Verify These addresses are accurate to 110...
ADDR CORE3 = 0x87A76C;
ADDR CORE7 = 0x71D0E8;
