#pragma once
#include <versions.h>
#include <winnt.h>

//I recognize this isn't an ideal constexpr setup.

constexpr engines::major ENGINE_TARGET = engines::major::CE;
constexpr engines::with_minor GAME_MINOR = engines::with_minor::halo_1_10;

//TODO: Something like "GetGameRegistryPath"
constexpr const char * GAME_REGISTRY_PATH =  R"(Software\Microsoft\Microsoft Games\Halo CE)";
