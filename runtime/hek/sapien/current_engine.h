#pragma once
#include <precompile.h>
#include <versions.h>
#include <winnt.h>

static const char* DEBUG_FILENAME = "tempera.sapien.debug.log";

namespace current_engine {
	const engines::major      ENGINE_TARGET = engines::major::HEK;
	const engines::with_minor GAME_MINOR    = engines::minor::sapien;
};
//TODO: Something like "GetGameRegistryPath"
const const char * GAME_REGISTRY_PATH =   R"(Software\Microsoft\Microsoft Games\Halo CE)";
