#pragma once
#include <versions.h>
#include <winnt.h>

//I recognize this isn't an ideal constexpr setup.

const engines::major ENGINE_TARGET = engines::major::CE;
const engines::with_minor GAME_MINOR = engines::with_minor::halo_1_09;

//TODO: Something like "GetGameRegistryPath"
const const char * GAME_REGISTRY_PATH =  R"(Software\Microsoft\Microsoft Games\Halo CE)";
