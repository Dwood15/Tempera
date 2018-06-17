#pragma once

#include <precompile.h>
#include "../../../../../../../BlamSauce/memory/memory_interface_base.hpp"

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_PTR(type, name, ...) static auto* const name = reinterpret_cast<type*>(__VA_ARGS__);
#define DUO_PTR(...) __VA_ARGS__
#define FUNC_PTR(name, value, ...) static auto const name  =  value
#define CAST_PTR(type, ptr)      reinterpret_cast<type>(ptr)

//////////////////////////////////////////////////////////////////////////
// Memory/FunctionInterface.cpp
static auto const RENDER_SKY = K_RENDER_SKY;
static auto const RENDER_WINDOW_CALL_HOOK_RENDER_SKY = K_RENDER_WINDOW_CALL_HOOK_RENDER_SKY;

static auto const RENDER_OBJECTS = K_RENDER_OBJECTS;
static auto const RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS = K_RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS;

static auto const STRUCTURE_RENDER_LIGHTMAPS = K_STRUCTURE_RENDER_LIGHTMAPS;
static auto const RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS = K_RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS;

static auto const WEATHER_PARTICLE_SYSTEMS_RENDER = K_WEATHER_PARTICLE_SYSTEMS_RENDER;
static auto const RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER = K_RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER;

static auto const RENDER_UI_WIDGETS = K_RENDER_UI_WIDGETS;
static auto const RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS = K_RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS;

static auto const RENDER_UI_CURSOR = K_RENDER_UI_CURSOR;
static auto const CALL_HOOK_RENDER_UI_CURSOR = K_CALL_HOOK_RENDER_UI_CURSOR;

static auto const INTERFACE_DRAW_SCREEN = K_INTERFACE_DRAW_SCREEN;
static auto const RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN = K_RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN;

static auto const UPDATE_UI_WIDGETS = K_UPDATE_UI_WIDGETS;
static auto const CALL_HOOK_UPDATE_UI_WIDGETS = K_CALL_HOOK_UPDATE_UI_WIDGETS;


//////////////////////////////////////////////////////////////////////////
// Common/GameSystems.cpp
static auto const QUERY_EXITFLAG_REG_CALL = K_QUERY_EXITFLAG_REG_CALL;
static auto const QUERY_EXITFLAG_REG = K_QUERY_EXITFLAG_REG;
static auto const RELEASE_RESOURCES_ON_EXIT_CALL = K_RELEASE_RESOURCES_ON_EXIT_CALL;
static auto const RELEASE_RESOURCES_ON_EXIT = K_RELEASE_RESOURCES_ON_EXIT;

	DATA_PTR(PE_DATA_SIZE,												0x215000, 0x171000);


//////////////////////////////////////////////////////////////////////////
// Common/DebugDump.cpp
static auto const WINMAIN_EXCEPTION_FILTER = K_WINMAIN_EXCEPTION_FILTER;
static auto const WINMAIN_EXCEPTION_FILTER_CALL = K_WINMAIN_EXCEPTION_FILTER_CALL;

static auto const RASTERIZER_DX9_SAVE_GAMMA = K_RASTERIZER_DX9_SAVE_GAMMA;
static auto const RASTERIZER_WINDOWS_PRESENT_FRAME = K_RASTERIZER_WINDOWS_PRESENT_FRAME;
static auto const SOUND_STOP_ALL = K_SOUND_STOP_ALL;

static auto *const VIRTUALPROTECT_LOCK = reinterpret_cast<void ***>(K_VIRTUALPROTECT_LOCK);
static auto *const VIRTUALPROTECT_OLD_PROTECT = reinterpret_cast<DWORD *>(K_VIRTUALPROTECT_OLD_PROTECT);;
