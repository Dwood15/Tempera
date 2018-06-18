#pragma once
#include <versions.h>
#include <winnt.h>
#include <precompile.h>

//I recognize this isn't an ideal constexpr setup.

static const char* DEBUG_FILENAME = "tempera.1_10.debug.log";
static constexpr engines::major ENGINE_TARGET = engines::major::CE;
static constexpr engines::minor GAME_MINOR = engines::minor::halo_1_10;
//TODO: Something like "GetGameRegistryPath"
static const char * GAME_REGISTRY_PATH =  R"(Software\Microsoft\Microsoft Games\Halo CE)";
static constexpr bool supported_target() { return true; }
