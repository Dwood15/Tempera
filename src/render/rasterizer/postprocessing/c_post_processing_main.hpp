#pragma once

#include <d3d9types.h>
#include <macros_generic.h>
#include <d3dx9math.h>
#include <enums/postprocessing.h>
#include "../../../math/real_math.h"
#include "../dx9/shaders/shader_postprocess_globals_definitions.hpp"
#include "../../../memory/packed_file.hpp"
#include "../GBuffer.hpp"
#include "../RenderTargetChain.hpp"
#include "../../../math/int_math.h"
#include "Interfaces/IPostProcessingCacheComponent.hpp"
#include "Interfaces/IPostProcessingUpdatable.hpp"

namespace Yelo {
	namespace Rasterizer {
		namespace PostProcessing {
			class c_post_processing_main :
				public IPostProcessingCacheComponent,
				public IPostProcessingUpdatable {
				/////////////////////////////////////////////////
				// static members
			private:
				static c_post_processing_main g_post_processing_main;

				/////////////////////////////////////////////////
				// static member accessors
			public:
				static c_post_processing_main &Instance();

				/////////////////////////////////////////////////
				// members
			private:
				struct {
					struct {
						bool is_ready;
						bool is_unloaded;
						bool is_disabled;
						PAD8;
					} m_flags;

					Enums::pp_component_status status;
					short : 16;
				} m_members;

			public:
				struct s_post_processing_globals {
					struct {
						bool has_postprocess_globals;
						char  : 8;
						short : 16;
					} m_flags;

					IDirect3DDevice9 *render_device;
					D3DCAPS9         device_caps;

					point2d      screen_dimensions;
					real_point2d screen_fov;
					real_point2d pixel_size;

					DX9::s_render_target_chain_scene
														scene_buffer_chain;
					DX9::s_render_target_chain secondary_buffer_chain;
					DX9::c_gbuffer             *gbuffer;

					c_packed_file                           shader_file;
					TagGroups::s_shader_postprocess_globals *
																			 map_postprocess_globals;

					D3DXMATRIX ortho_proj_matrix;

					struct {
						real_point2d scaled_ratio;
						real_point2d default_ratio;
						real_point2d value;
					}          m_hud;

					struct {
						real near_clip;
						real far_clip;
					}          m_clipping;

					real delta_time;

					struct {
						bool player_has_control;
						bool in_cinematic;
						bool local_player_in_vehicle;
						bool local_player_zoomed;
					}    m_activation_variables;
				} m_globals;

				/////////////////////////////////////////////////
				// member accessors
			public:
				s_post_processing_globals &Globals();

				bool IsReady();

				bool IsUnloaded();

				/////////////////////////////////////////////////
				// IPostProcessingComponent
			public:
				void Initialize();

				void Dispose();

				void InitializeResources_Base(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *parameters);

				void OnLostDevice_Base();

				void OnResetDevice_Base(D3DPRESENT_PARAMETERS *parameters);

				void ReleaseResources_Base();

				void Unload();

				void Load();

				/////////////////////////////////////////////////
				// IPostProcessingCacheComponent
			public:
				void Initialize_Cache();

				void Dispose_Cache();

				/////////////////////////////////////////////////
				// system setup
			private:
				void GetRenderProperties(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *parameters);

				bool CreateScreenBufferChain();

				void DestroyScreenBufferChain();

				bool CreateSecondaryBufferChain();

				void DestroySecondaryBufferChain();

				void GetPostprocessGlobalsTag();

				void ClearPostprocessGlobalsTag();

				void OpenGlobalShaderFile();

				void CloseGlobalShaderFile();

				void UpdateStatus();

				/////////////////////////////////////////////////
				// system application
			public:
				void PollUpdate();
			};

		};
	};
};
