#pragma once
#include <versions.h>
#include <winnt.h>

static_assert(false);
//I recognize this isn't an ideal constexpr setup.
constexpr engines::major ENGINE_TARGET = engines::major::PC;
constexpr engines::with_minor GAME_MINOR = engines::with_minor::halo_1_10;

//TODO: Something like "GetGameRegistryPath"
constexpr const char * GAME_REGISTRY_PATH =  R"(Software\Microsoft\Microsoft Games\Halo)";
