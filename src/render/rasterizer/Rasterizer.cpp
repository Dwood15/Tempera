/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Rasterizer.hpp"
#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/interface/ui_video_screen.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Game/EngineFunctions.hpp"
#include "Common/GameSystems.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_RASTERIZER
#include "Memory/_EngineLayout.inl"

	namespace Rasterizer
	{
#include <YeloLib/Halo1/rasterizer/rasterizer_upgrades.inl>

		s_rasterizer_config* RasterizerConfig()			PTR_IMP_GET2(rasterizer_config);
		s_rasterizer_globals* RasterizerGlobals()		PTR_IMP_GET2(rasterizer_globals);
#pragma region DebugOptions
		s_rasterizer_debug_options* DebugOptions()		PTR_IMP_GET2(rasterizer_debug_data);

		struct rasterizer_debug_table
		{
			size_t field;
			uint32 index;
		};
		static const rasterizer_debug_table k_rasterizer_debug_table[] = {
			{offsetof(s_rasterizer_debug_options, stats),									0xC},
			{offsetof(s_rasterizer_debug_options, mode),									0xD},
			{offsetof(s_rasterizer_debug_options, smart),									0xF},
			{offsetof(s_rasterizer_debug_options, debug_model_vertices),					0x10},
			{offsetof(s_rasterizer_debug_options, debug_model_lod),							0x11},
			{offsetof(s_rasterizer_debug_options, debug_transparents),						0x12},
			{offsetof(s_rasterizer_debug_options, debug_meter_shader),						0x13},
			{offsetof(s_rasterizer_debug_options, models),									0x14},
			{offsetof(s_rasterizer_debug_options, model_transparents),						0x15},
			{offsetof(s_rasterizer_debug_options, draw_first_person_weapon_first),			0x16},
			{offsetof(s_rasterizer_debug_options, stencil_mask),							0x17},
			{offsetof(s_rasterizer_debug_options, environment),								0x18},
			{offsetof(s_rasterizer_debug_options, environment_lightmaps),					0x19},
			{offsetof(s_rasterizer_debug_options, environment_shadows),						0x1A},
			{offsetof(s_rasterizer_debug_options, environment_diffuse_lights),				0x1B},
			{offsetof(s_rasterizer_debug_options, environment_decals),						0x1D},
			{offsetof(s_rasterizer_debug_options, environment_specular_lights),				0x1E},
			{offsetof(s_rasterizer_debug_options, environment_specular_lightmaps),			0x1F},
			{offsetof(s_rasterizer_debug_options, environment_reflection_lightmap_mask),	0x20},
			{offsetof(s_rasterizer_debug_options, environment_reflection_mirrors),			0x21},
			{offsetof(s_rasterizer_debug_options, environment_reflections),					0x22},
			{offsetof(s_rasterizer_debug_options, environment_transparents),				0x23},
			{offsetof(s_rasterizer_debug_options, environment_fog),							0x24},
			{offsetof(s_rasterizer_debug_options, environment_fog_screen),					0x25},
			{offsetof(s_rasterizer_debug_options, water),									0x26},
			{offsetof(s_rasterizer_debug_options, lens_flares),								0x27},
			{offsetof(s_rasterizer_debug_options, dynamic_unlit_geometry),					0x28},
			{offsetof(s_rasterizer_debug_options, dynamic_lit_geometry),					0x29},
			{offsetof(s_rasterizer_debug_options, dynamic_screen_geometry),					0x2A},
			{offsetof(s_rasterizer_debug_options, hud_motion_sensor),						0x2B},
			{offsetof(s_rasterizer_debug_options, detail_objects),							0x2C},
			{offsetof(s_rasterizer_debug_options, debug_geometry),							0x2D},
			{offsetof(s_rasterizer_debug_options, debug_geometry_multipass),				0x2E},
			{offsetof(s_rasterizer_debug_options, bump_mapping),							0x31},
			{offsetof(s_rasterizer_debug_options, lightmap_ambient),						0x32},
			//{offsetof(s_rasterizer_debug_options, ), 0x33},
			{offsetof(s_rasterizer_debug_options, lightmaps_incident),						0x34},
			{offsetof(s_rasterizer_debug_options, lightmaps_filtering),						0x35},
			{offsetof(s_rasterizer_debug_options, model_lightning_ambient),					0x36},
			{offsetof(s_rasterizer_debug_options, environment_alpha_testing),				0x37},
			{offsetof(s_rasterizer_debug_options, environment_specular_mask),				0x38},
			{offsetof(s_rasterizer_debug_options, shadows_convolution),						0x39},
			{offsetof(s_rasterizer_debug_options, shadows_debug),							0x3A},
			{offsetof(s_rasterizer_debug_options, water_mipmapping),						0x3B},
			{offsetof(s_rasterizer_debug_options, active_camouflage),						0x3C},
			{offsetof(s_rasterizer_debug_options, active_camouflage_multipass),				0x3D},
			{offsetof(s_rasterizer_debug_options, plasma_energy),							0x3E},
			{offsetof(s_rasterizer_debug_options, lens_flares_occlusion),					0x3F},
			{offsetof(s_rasterizer_debug_options, lens_flares_occlusion_debug),				0x40},
			{offsetof(s_rasterizer_debug_options, ray_of_buddha),							0x41},
			{offsetof(s_rasterizer_debug_options, screen_flashes),							0x42},
			{offsetof(s_rasterizer_debug_options, screen_effects),							0x43},
			{offsetof(s_rasterizer_debug_options, DXTC_noise),								0x44},
			{offsetof(s_rasterizer_debug_options, soft_filter),								0x45},
			{offsetof(s_rasterizer_debug_options, secondary_render_target_debug),			0x46},
			{offsetof(s_rasterizer_debug_options, profile_log),								0x47},
			{offsetof(s_rasterizer_debug_options, detail_objects_offset_multiplier),		0x48},
			{offsetof(s_rasterizer_debug_options, zbias), 									0x49},
			{offsetof(s_rasterizer_debug_options, zoffset),									0x4A},
			{offsetof(s_rasterizer_debug_options, force_all_player_views_to_default_player),0x4B},
			{offsetof(s_rasterizer_debug_options, safe_frame_bounds),						0x4C},
			{offsetof(s_rasterizer_debug_options, zsprites),								0x4E},
			{offsetof(s_rasterizer_debug_options, filthy_decal_fog_hack),					0x4F},
			{offsetof(s_rasterizer_debug_options, splitscreen_VB_optimization),				0x50},
			{offsetof(s_rasterizer_debug_options, profile_print_locks),						0x51},
			{offsetof(s_rasterizer_debug_options, profile_objectlook_time),					0x52},
			//{offsetof(rasterizer_debug_options, ), 0x53},
			{offsetof(s_rasterizer_debug_options, pad3_scale),								0x54},
			{offsetof(s_rasterizer_debug_options, f0),										0x55},
			{offsetof(s_rasterizer_debug_options, f1),										0x56},
			{offsetof(s_rasterizer_debug_options, f2),										0x57},
			{offsetof(s_rasterizer_debug_options, f3),										0x58},
			{offsetof(s_rasterizer_debug_options, f4),										0x59},
			{offsetof(s_rasterizer_debug_options, f5),										0x5A},
			//{offsetof(s_rasterizer_debug_options, ), 0x},
		};
#pragma endregion

		s_rasterizer_frame_parameters* FrameParameters()	PTR_IMP_GET2(rasterizer_frame_params);

		static char g_screenshot_folder[MAX_PATH] = "screenshots\\";
		static s_rasterizer_resolution g_resolution_list[64];

#pragma region Settings
		c_settings_container::c_upgrades_container::c_upgrades_container()
			: Configuration::c_configuration_container("Upgrades")
			, m_maximum_rendered_triangles("MaximumRenderedTriangles", true)
#if PLATFORM_VERSION <= 0x1090
			, m_model_node_stretching_fix("MaximumModelNodes", true)
#endif
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::c_upgrades_container::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const>
			{
				&m_maximum_rendered_triangles,
#if PLATFORM_VERSION <= 0x1090
				&m_model_node_stretching_fix,
#endif
			};
		}

		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Rasterizer")
#if PLATFORM_VERSION <= 0x1090
			, m_use_nvidia_camo("UseNvidiaCamo", false)
#endif
			, m_upgrades()
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
#if PLATFORM_VERSION <= 0x1090
				&m_use_nvidia_camo,
#endif
				&m_upgrades
			};
		}
		
		void c_settings_rasterizer::PostLoad()
		{
			auto& settings_instance = Get();

#if PLATFORM_VERSION <= 0x1090
			// when 0x637D50 (1.09) is 1, the basic active camouflage is used; at 0x51ABB2 (1.09) it is forced to 1 when an nVidia card is detected
			// if the user changes this in their settings they need to restart the game for it to take effect
			GET_PTR(NVIDIA_USE_BASIC_CAMO_TOGGLE) = settings_instance.m_use_nvidia_camo;

			if(settings_instance.m_upgrades.m_model_node_stretching_fix)
			{
				g_render_upgrades.InitializeMaximumNodesPerModelFixes();
			}
#endif
				
			if(settings_instance.m_upgrades.m_maximum_rendered_triangles)
			{
				g_render_upgrades.InitializeRenderedTrianglesUpgrade();
			}
		}
#pragma endregion

		static void InitializeRasterizerGeometryUpgrades()
		{
			return; // TODO: WIP code

			GET_PTR(RASTERIZER_DYNAMIC_GEOMETRY_INITIALIZE__CreateIndexBuffer_Length_ARG) =
				sizeof(struct rasterizer_triangle)*Enums::k_rasterizer_maximum_dynamic_triangles;
		}

#if PLATFORM_VERSION <= 0x1090
		static void SetupResolutions()
		{
			if(!DX9::Direct3D9())
				return;

			// get the current screen resolution
			RECT desktop_dimensions;
			HWND desktop_handle = GetDesktopWindow();
			GetWindowRect(desktop_handle, &desktop_dimensions);

			// get the number of adapter modes
			UINT adapter_mode_index = DX9::Direct3D9()->GetAdapterModeCount(GET_PTR(RASTERIZER_DEVICE_ADAPTER_INDEX), D3DFMT_X8R8G8B8);
			if(!adapter_mode_index)
				return;

			// loop through all of the adapter modes
			while(adapter_mode_index)
			{
				adapter_mode_index--;
				D3DDISPLAYMODE display_mode;
				HRESULT success = DX9::Direct3D9()->EnumAdapterModes(GET_PTR(RASTERIZER_DEVICE_ADAPTER_INDEX), D3DFMT_X8R8G8B8, adapter_mode_index, &display_mode);
				
				if(!SUCCEEDED(success))
					continue;

				// if we are windowed, dont add resolutions that are greater than the desktop size
				bool add_mode = true;
				if(DX9::D3DPresetParams()->Windowed)
					add_mode = (display_mode.Width < (UINT)desktop_dimensions.right) && (display_mode.Height < (UINT)desktop_dimensions.bottom);

				// add the resolution
				if(add_mode)
					blam::ui_video_screen_add_resolution(display_mode.Width, display_mode.Height, display_mode.RefreshRate);
			}
		}
#endif

		// release direct3D resources before the device is destroyed
		static void RasterizerDisposeHook()
		{			
			Yelo::Main::s_dx_component* components;
			const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

			for(Yelo::int32 x = 0; x <= component_count; x++)
				components[x].Release();

			// Return the code flow back to the game
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_DISPOSE);
			__asm	call RETN_ADDRESS
		}

#pragma warning( push )
#pragma warning( disable : 4311 ) // pointer truncation
#pragma warning( disable : 4312 ) // conversion from 'unsigned long' to 'void *' of greater size
		void Initialize()
		{
			c_settings_rasterizer::Register(Settings::Manager());

			Render::Initialize();
		
			// hook the calls to rasterizer_dispose
			Memory::WriteRelativeCall(&RasterizerDisposeHook, GET_FUNC_VPTR(RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER));
			Memory::WriteRelativeCall(&RasterizerDisposeHook, GET_FUNC_VPTR(RASTERIZER_DISPOSE_CALL_FROM_SHELL));

			size_t address = CAST_PTR(size_t, Rasterizer::DebugOptions());
			for(const auto& rdt : k_rasterizer_debug_table)
			{
				// this is the only time we should be modifying the hs definitions
				// outside of the ScriptLibrary code, so do some cast magic
				Scripting::hs_global_definition* global_def = CAST_QUAL(Scripting::hs_global_definition*, 
					&(Scripting::HSExternalGlobals()[rdt.index]) );	// get the hs global definition we're about to fix up

				global_def->address = CAST_PTR(void*, address + rdt.field);// fix the global definition's address to point to the correct memory
			}
			
#if PLATFORM_VERSION <= 0x1090
			// update the resolution definition array length
			// definition count has been increased to 64 so that ridiculous amounts of resolutions in the future are accommodated
			GET_PTR(RESOLUTION_LIST_COUNT) = NUMBEROF(g_resolution_list);

			// redirect all resolution definition pointers to the new array
			for(auto ptr : K_RESOLUTION_LIST_X_REFERENCES)
				*ptr = &g_resolution_list[0].width;
			for(auto ptr : K_RESOLUTION_LIST_Y_REFERENCES)
				*ptr = &g_resolution_list[0].height;
			for(auto ptr : K_RESOLUTION_LIST_STRING_REFERENCES)
				*ptr = &g_resolution_list[0].resolution_string;

			*GET_PTR(RESOLUTION_LIST_STRING_NULL_REFERENCE) = &(g_resolution_list[0].resolution_string[15]);

			for(auto ptr : K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCES)
				*ptr = &g_resolution_list[0].refresh_rate_count;
			for(auto ptr : K_RESOLUTION_LIST_REFRESH_RATE_REFERENCES)
				*ptr = &g_resolution_list[0].refresh_rates;

			// replace the original resolution populator with the new one
			Memory::WriteRelativeCall(&SetupResolutions, GET_FUNC_VPTR(RESOLUTION_LIST_SETUP_RESOLUTIONS_CALL), true);
#endif

			// make the screenshot function use a unique subfolder
			tag_string screenshots_folder;
			GetTimeStampStringForFile(screenshots_folder);
			strcat_s(g_screenshot_folder, sizeof(g_screenshot_folder), screenshots_folder);
			for(auto ptr : K_SCREENSHOT_FOLDER_REFERENCES)
				*ptr = &g_screenshot_folder[0];
		}
#pragma warning( pop )

		void Dispose()
		{
			Render::Dispose();
			
			c_settings_rasterizer::Unregister(Settings::Manager());
		}

		void Update()
		{
#if !defined(DX_WRAPPER)
			// only thing that changes is the vertex type (texture or color)
			// so I just made a global render states function
			// and put the setFVF (vertex change function) in each class
			//Render::SetRenderStates();

			// TODO: need to make this a naked function in order for the compiler to possibly not push & pop edx
			__asm {
				call DX9::Direct3DDevice
				mov edx, [eax]
			}
#endif
		}
	};

	namespace Render
	{
		s_render_window* RenderWindow()							PTR_IMP_GET2(render_window);
		s_render_globals* RenderGlobals()						PTR_IMP_GET2(render_globals);
		s_render_target* GlobalRenderTargets()					PTR_IMP_GET2(global_render_targets);
		s_structure_render_globals* StructureRenderGlobals()	PTR_IMP_GET2(structure_render_globals);

		static bool g_is_rendering_reflection = false;
		bool IsRenderingReflection() { return g_is_rendering_reflection; }

		static void PLATFORM_API RenderWindowHook(const uint16 local_player_index
			, void* render_camera
			, void* render_frustum
			, void* rasterizer_camera
			, void* rasterizer_frustum
			, void* rasterizer_target
			, const bool is_mirror)
		{
			g_is_rendering_reflection = true;
			blam::render_window(local_player_index, render_camera, render_frustum, rasterizer_camera, rasterizer_frustum, rasterizer_target, is_mirror);
			g_is_rendering_reflection = false;
		}

		void Initialize()
		{
			// TODO: If using DX_WRAPPER, refer to the DxWrapper.cpp file 
			// instead of hooking the game render loop
#if !defined(DX_WRAPPER)
			Memory::WriteRelativeCall(&Rasterizer::Update, GET_FUNC_VPTR(RENDER_WINDOW_END_HOOK));
#endif

			Memory::WriteRelativeCall(&RenderWindowHook, GET_FUNC_VPTR(RENDER_WINDOW_REFLECTION_CALL), true);
		}

		void Dispose()
		{
		}

		void SetRenderStates()
		{
			DX9::Direct3DDevice()->SetVertexShader(nullptr);
			DX9::Direct3DDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
			DX9::Direct3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			DX9::Direct3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			DX9::Direct3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			DX9::Direct3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
	};
};

#endif