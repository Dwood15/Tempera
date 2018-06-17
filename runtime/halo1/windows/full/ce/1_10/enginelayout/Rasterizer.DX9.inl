#include <d3d9types.h>
#include <d3d9caps.h>
#include <d3d9.h>

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


//////////////////////////////////////////////////////////////////////////
// DX9.cpp
static auto *const Params = reinterpret_cast<D3DPRESENT_PARAMETERS *>(K_PARAMS);
static auto *const Caps = reinterpret_cast<D3DCAPS9 *>(K_CAPS);
static auto **const D3D = reinterpret_cast<IDirect3D9 **>(K_D3D);;
static auto **const Device = reinterpret_cast<IDirect3DDevice9 **>(K_DEVICE);;
static auto **const DInput8 = reinterpret_cast<IDirectInput8 **>(K_DINPUT8);;
static auto **const DInput8DeviceKeyboard = reinterpret_cast<IDirectInputDevice8 **>(K_DINPUT8DEVICEKEYBOARD);;
static auto **const DInput8DeviceMouse = reinterpret_cast<IDirectInputDevice8 **>(K_DINPUT8DEVICEMOUSE);;
static auto **const DInput8DeviceJoysticks = reinterpret_cast<IDirectInputDevice8 **>(K_DINPUT8DEVICEJOYSTICKS);;


//////////////////////////////////////////////////////////////////////////
// GBuffer.cpp
static auto *const VSF_TABLE_START_REFERENCE = reinterpret_cast<DX9::s_vsf_entry **>(K_VSF_TABLE_START_REFERENCE);

static auto const RENDER_OBJECT_LIST_HOOK = K_RENDER_OBJECT_LIST_HOOK;
static auto const RENDER_OBJECT_LIST_HOOK_RETN = K_RENDER_OBJECT_LIST_HOOK_RETN;

static auto const RENDER_OBJECT_LIST_END_HOOK = K_RENDER_OBJECT_LIST_END_HOOK;

static auto const RENDER_OBJECT_OBJECT_LOD_HOOK = K_RENDER_OBJECT_OBJECT_LOD_HOOK;
static auto const RENDER_OBJECT_OBJECT_LOD_HOOK_RETN = K_RENDER_OBJECT_OBJECT_LOD_HOOK_RETN;

static auto const FIRST_PERSON_WEAPON_DRAW_HOOK = K_FIRST_PERSON_WEAPON_DRAW_HOOK;
static auto const FIRST_PERSON_WEAPON_DRAW_HOOK_RETN = K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN;

static auto const COMMAND_CAMERA_SET_HOOK = K_COMMAND_CAMERA_SET_HOOK;
static auto const COMMAND_CAMERA_SET_HOOK_RETN = K_COMMAND_CAMERA_SET_HOOK_RETN;

static auto const COMMAND_SWITCH_BSP_HOOK = K_COMMAND_SWITCH_BSP_HOOK;
static auto const COMMAND_SWITCH_BSP_HOOK_RETN = K_COMMAND_SWITCH_BSP_HOOK_RETN;

static auto const COMMAND_GAME_SAVE_HOOK = K_COMMAND_GAME_SAVE_HOOK;
static auto const COMMAND_GAME_SAVE_HOOK_RETN = K_COMMAND_GAME_SAVE_HOOK_RETN;

static auto const RENDER_OBJECTS_TRANSPARENT = K_RENDER_OBJECTS_TRANSPARENT;
static auto const RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK = K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK;
static auto const RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN = K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN;

static auto const RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK = K_RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK;
static auto const RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK = K_RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK;

//////////////////////////////////////////////////////////////////////////
// DX9/rasterizer_dx9_shaders_vshader9.cpp
static auto *const rasterizer_dx9_effect_collection = reinterpret_cast<s_rasterizer_dx9_effect_collection *>(K_RASTERIZER_DX9_EFFECT_COLLECTION);
static auto *const rasterizer_effect_shaders = reinterpret_cast<s_effect_shader_entry *>(K_RASTERIZER_EFFECT_SHADERS);
static auto *const vsf_table = reinterpret_cast<s_vsf_entry *>(K_VSF_TABLE);

//////////////////////////////////////////////////////////////////////////
// DeviceHooks.cpp
static auto const RASTERIZER_D3D_CREATE_DEVICE_HOOK = K_RASTERIZER_D3D_CREATE_DEVICE_HOOK;
static auto const RASTERIZER_D3D_CREATE_DEVICE_RETN = K_RASTERIZER_D3D_CREATE_DEVICE_RETN;
static auto const RASTERIZER_D3D_RESET_DEVICE_HOOK = K_RASTERIZER_D3D_RESET_DEVICE_HOOK;
static auto const RASTERIZER_D3D_BEGIN_SCENE_CALL = K_RASTERIZER_D3D_BEGIN_SCENE_CALL;
static auto const RASTERIZER_D3D_END_SCENE_CALL = K_RASTERIZER_D3D_END_SCENE_CALL;

static auto const RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL = K_RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL;
static auto const RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL = K_RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL;

	static void** K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALLS[] = {
		reinterpret_cast<void **>(K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_0),
		reinterpret_cast<void **>(K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_1),
	};
static auto const RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL = K_RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL;
