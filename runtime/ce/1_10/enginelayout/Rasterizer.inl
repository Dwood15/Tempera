#pragma once

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_DPTR(type, name, ...) static auto** const name = reinterpret_cast<type**>(__VA_ARGS__);
#define ENGINE_PTR(type, name, ...) static auto* const name = reinterpret_cast<type*>(__VA_ARGS__);
#define DUO_PTR(...) __VA_ARGS__
#define FUNC_PTR(name, value, ...) static auto const name  =  value
#define CAST_PTR(type, ptr)      reinterpret_cast<type>(ptr)

ADDR K_RASTERIZER_CONFIG                       = 0x6BDDF8;
ADDR K_RASTERIZER_GLOBALS                      = 0x637CE8;
ADDR K_RASTERIZER_DEBUG_DATA                   = 0x6249C0;
ADDR K_RASTERIZER_FRAME_PARAMS                 = 0x75C570;
ADDR K_RASTERIZER_DISPOSE                      = 0x51BD00;
ADDR K_RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER = 0x51ACE0;
ADDR K_RASTERIZER_DISPOSE_CALL_FROM_SHELL      = 0x544459;

ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_0         = 0x51C2F3;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_1         = 0x52A0BF;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_2         = 0x52A116;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_3         = 0x52A16E;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_4         = 0x52A1CC;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_5         = 0x52A26B;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8_0 = 0x51C26A;

ADDR K_RENDER_GLOBALS                              = 0x75E460;
ADDR K_GLOBAL_RENDER_TARGETS                       = 0x638A10;
ADDR K_STRUCTURE_RENDER_GLOBALS                    = 0x67EA98;
ADDR K_RENDER_WINDOW_REFLECTION_CALL               = 0x50F680;
ADDR K_RENDER_WINDOW_REFLECTION_CALL_RETN          = 0x50F685;
ADDR K_RENDER_WINDOW_END_HOOK                      = 0x51B958;
ADDR K_BSP_LIGHTMAP_INDEX_HOOK                     = 0x5562DA;
ADDR K_BSP_LIGHTMAP_INDEX_RETN                     = 0x5562E0;
ADDR K_SET_LIGHTMAP_SAMPLER_CALL                   = 0x522B6C;
ADDR K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK = 0x4F5F84;
ADDR K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN = 0x4F5F8A;
ADDR K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_HOOK          = 0x541C5F;
ADDR K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN          = 0x541C66;
ADDR K_FOG_PLANAR_SKY_TAG_INDEX_HOOK               = 0x5587BC;
ADDR K_FOG_PLANAR_SKY_TAG_INDEX_RETN               = 0x5587C2;
ADDR K_RENDER_SKY_SKY_TAG_INDEX_HOOK               = 0x5144FE;
ADDR K_RENDER_SKY_SKY_TAG_INDEX_RETN               = 0x514505;
ADDR K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_HOOK       = 0x556960;
ADDR K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_RETN       = 0x55696A;

ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_0 = 0x556D87;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_1 = 0x556DF7;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_2 = 0x556E5A;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_3 = 0x556F77;

ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_0  = 0x50FAAE;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_1  = 0x50FC0C;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_2  = 0x50FC4E;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_3  = 0x50FC87;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_4  = 0x50FCB9;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_5  = 0x50FCE3;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_6  = 0x50FD1E;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_7  = 0x50FE2F;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_8  = 0x50FE50;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_9  = 0x555C40;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_10 = 0x555D7C;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_11 = 0x555E66;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_12 = 0x555F46;

ADDR K_RASTERIZER_DEVICE_ADAPTER_INDEX                                      = 0x6B8418;
ADDR K_RASTERIZER_DYNAMIC_GEOMETRY_INITIALIZE__CREATEINDEXBUFFER_LENGTH_ARG = 0x51F2C2;


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
namespace Rasterizer {
	static real_matrix3x4 **K_VSH_CONSTANTS__NODEMATRICES_REFERENCES[]        = {
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_0),
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_1),
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_2),
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_3),
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_4),
		reinterpret_cast<real_matrix3x4 **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_5),
	};

	static void           **K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8[] = {
		reinterpret_cast<void **>(K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8_0),
	};

	// static auto *const rasterizer_config =;
	// static auto *const rasterizer_globals = ;

	static auto const RASTERIZER_DISPOSE                      = K_RASTERIZER_DISPOSE;
	static auto const RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER = K_RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER;
	static auto const RASTERIZER_DISPOSE_CALL_FROM_SHELL      = K_RASTERIZER_DISPOSE_CALL_FROM_SHELL;

	static void           **K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST[]   = {
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_0),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_1),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_2),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_3),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_4),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_5),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_6),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_7),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_8),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_9),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_10),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_11),
		reinterpret_cast<void **>(K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_12),
	};

	static unsigned short *K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_LIST[] = {
		reinterpret_cast<unsigned short *>(K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_0),
		reinterpret_cast<unsigned short *>(K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_1),
		reinterpret_cast<unsigned short *>(K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_2),
		reinterpret_cast<unsigned short *>(K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_3),
	};

	static auto *const RASTERIZER_DYNAMIC_GEOMETRY_INITIALIZE__CreateIndexBuffer_Length_ARG = reinterpret_cast<DWORD *>(K_RASTERIZER_DYNAMIC_GEOMETRY_INITIALIZE__CREATEINDEXBUFFER_LENGTH_ARG, PTR_NULL);;

	static auto *const RASTERIZER_DEVICE_ADAPTER_INDEX = reinterpret_cast<DWORD *>(K_RASTERIZER_DEVICE_ADAPTER_INDEX, PTR_NULL);;

	static char **K_SCREENSHOT_FOLDER_REFERENCES[] = {
		// reinterpret_cast<char **>(K_SCREENSHOT_FOLDER_REFERENCE_0),
		// reinterpret_cast<char **>(K_SCREENSHOT_FOLDER_REFERENCE_1),
	};
};

namespace Render {
	using namespace;
	static auto *const global_render_targets = reinterpret_cast<Yelo::Render::s_render_target *>(K_GLOBAL_RENDER_TARGETS, PTR_NULL);

	static auto const RENDER_WINDOW_REFLECTION_CALL      = K_RENDER_WINDOW_REFLECTION_CALL;
	static auto const RENDER_WINDOW_REFLECTION_CALL_RETN = K_RENDER_WINDOW_REFLECTION_CALL_RETN;

	static auto const RENDER_WINDOW_END_HOOK = K_RENDER_WINDOW_END_HOOK;

	namespace Lightmaps {
		static auto const BSP_LIGHTMAP_INDEX_HOOK                     = K_BSP_LIGHTMAP_INDEX_HOOK;
		static auto const BSP_LIGHTMAP_INDEX_RETN                     = K_BSP_LIGHTMAP_INDEX_RETN;
		static auto const SET_LIGHTMAP_SAMPLER_CALL                   = K_SET_LIGHTMAP_SAMPLER_CALL;
		static auto const OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK = K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK;
		static auto const OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN = K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN;
	};

	namespace Sky {
		static auto const FOG_ATMOSPHERIC_SKY_TAG_INDEX_HOOK    = K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_HOOK;
		static auto const FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN    = K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN;
		static auto const FOG_PLANAR_SKY_TAG_INDEX_HOOK         = K_FOG_PLANAR_SKY_TAG_INDEX_HOOK;
		static auto const FOG_PLANAR_SKY_TAG_INDEX_RETN         = K_FOG_PLANAR_SKY_TAG_INDEX_RETN;
		static auto const RENDER_SKY_SKY_TAG_INDEX_HOOK         = K_RENDER_SKY_SKY_TAG_INDEX_HOOK;
		static auto const RENDER_SKY_SKY_TAG_INDEX_RETN         = K_RENDER_SKY_SKY_TAG_INDEX_RETN;
		static auto const VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_HOOK = K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_HOOK;
		static auto const VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_RETN = K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_RETN;
	};
};

