/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/main/main_structures.hpp>

#include "Settings/Settings.hpp"
#include "Interface/GameUI.hpp"
#include "Game/GameState.hpp"
#include "Game/Camera.hpp"
#include "Game/Players.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/////////////////////////////////////////////////
		// static members
		c_post_processing_main c_post_processing_main::g_post_processing_main;

		/////////////////////////////////////////////////
		// static member accessors
		c_post_processing_main& c_post_processing_main::Instance() { return g_post_processing_main; }

		/////////////////////////////////////////////////
		// member accessors
		c_post_processing_main::s_post_processing_globals& c_post_processing_main::Globals()
		{
			return m_globals;
		}

		bool c_post_processing_main::IsReady()
		{
			return m_members.m_flags.is_ready;
		}

		bool c_post_processing_main::IsUnloaded()
		{
			return m_members.m_flags.is_unloaded;
		}

		/////////////////////////////////////////////////
		// IPostProcessingComponent
		void c_post_processing_main::Initialize() 
		{
			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_disabled = false;
			m_members.m_flags.is_unloaded = false;
			m_members.status = Enums::pp_component_status_uninitialised;

			OpenGlobalShaderFile();

			m_globals.m_flags.has_postprocess_globals = false;

			m_globals.gbuffer = nullptr;
			m_globals.map_postprocess_globals = nullptr;
			m_globals.render_device = nullptr;
		}

		void c_post_processing_main::Dispose() 
		{
			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_disabled = false;
			m_members.m_flags.is_unloaded = false;
			m_members.status = Enums::pp_component_status_uninitialised;

			CloseGlobalShaderFile();

			m_globals.m_flags.has_postprocess_globals = false;

			m_globals.gbuffer = nullptr;
			m_globals.map_postprocess_globals = nullptr;
			m_globals.render_device = nullptr;
		}

		void c_post_processing_main::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
		{
			GetRenderProperties(device, parameters);

			if(CreateScreenBufferChain() && CreateSecondaryBufferChain()) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		void c_post_processing_main::OnLostDevice_Base()
		{
			DestroyScreenBufferChain();
			DestroySecondaryBufferChain();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		void c_post_processing_main::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
		{
			GetRenderProperties(DX9::Direct3DDevice(), parameters);

			if(!m_members.m_flags.is_unloaded)
			{
				if(CreateScreenBufferChain() && CreateSecondaryBufferChain()) { m_members.status = Enums::pp_component_status_initialised; }
				else { m_members.status = Enums::pp_component_status_initialisation_failed; }
			}

			UpdateStatus();
		}

		void c_post_processing_main::ReleaseResources_Base()
		{
			// TODO: this function is fully equivalent to OnLostDevice_Base...

			DestroyScreenBufferChain();
			DestroySecondaryBufferChain();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		void c_post_processing_main::Unload()
		{
			CloseGlobalShaderFile();

			DestroyScreenBufferChain();
			DestroySecondaryBufferChain();

			m_members.status = Enums::pp_component_status_uninitialised;
			m_members.m_flags.is_unloaded = true;

			UpdateStatus();
		}

		void c_post_processing_main::Load()
		{
			OpenGlobalShaderFile();

			if(CreateScreenBufferChain() && CreateSecondaryBufferChain()) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			m_members.m_flags.is_unloaded = false;

			UpdateStatus();
		}

		/////////////////////////////////////////////////
		// IPostProcessingCacheComponent
		void c_post_processing_main::Initialize_Cache()
		{
			GetPostprocessGlobalsTag();
		}

		void c_post_processing_main::Dispose_Cache()
		{
			ClearPostprocessGlobalsTag();
		}

		/////////////////////////////////////////////////
		// system setup
		void c_post_processing_main::GetRenderProperties(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
		{
			m_globals.render_device = device;

			m_globals.render_device->GetDeviceCaps(&m_globals.device_caps);

			m_globals.screen_dimensions.x = parameters->BackBufferWidth;
			m_globals.screen_dimensions.y = parameters->BackBufferHeight;
			
			m_globals.pixel_size.x = 1.0f / parameters->BackBufferWidth;
			m_globals.pixel_size.y = 1.0f / parameters->BackBufferHeight;

			real_point2d scale = {1.0f, 1.0f};

			const real old_ratio = 4.0f / 3.0f;
			const real new_ratio = (real)parameters->BackBufferWidth / (real)parameters->BackBufferHeight;

			if (new_ratio > old_ratio)
				scale.x = old_ratio / new_ratio;
			else
				scale.y = new_ratio / old_ratio;

			m_globals.m_hud.scaled_ratio = scale;
			m_globals.m_hud.default_ratio.x = m_globals.m_hud.default_ratio.y = 1.0f;

			D3DXMATRIX ortho_translation;
			D3DXMatrixTranslation(&ortho_translation, -1.0f, 1.0f, 0);
			D3DXMatrixOrthoLH (&m_globals.ortho_proj_matrix, 
				m_globals.screen_dimensions.x, 
				m_globals.screen_dimensions.y, 
				1.0f, 10.0f);
			D3DXMatrixMultiply(&m_globals.ortho_proj_matrix, &m_globals.ortho_proj_matrix, &ortho_translation);

			m_globals.gbuffer = &DX9::c_gbuffer_system::GBuffer();
		}

		bool c_post_processing_main::CreateScreenBufferChain()
		{
			if(Rasterizer::RasterizerConfig()->disable_render_targets || Rasterizer::RasterizerConfig()->disable_alpha_render_targets)
				return false;

			return m_globals.scene_buffer_chain.AllocateResources(m_globals.render_device,
				m_globals.screen_dimensions.x,
				m_globals.screen_dimensions.y);
		}

		void c_post_processing_main::DestroyScreenBufferChain()
		{
			m_globals.scene_buffer_chain.ReleaseResources();
		}

		bool c_post_processing_main::CreateSecondaryBufferChain()
		{
			if(Rasterizer::RasterizerConfig()->disable_render_targets || Rasterizer::RasterizerConfig()->disable_alpha_render_targets)
				return false;

			return m_globals.secondary_buffer_chain.AllocateResources(m_globals.render_device,
				m_globals.screen_dimensions.x,
				m_globals.screen_dimensions.y,
			D3DFMT_A8R8G8B8);
		}

		void c_post_processing_main::DestroySecondaryBufferChain()
		{
			m_globals.secondary_buffer_chain.ReleaseResources();
		}

		void c_post_processing_main::GetPostprocessGlobalsTag()
		{
			TagGroups::s_tag_iterator iter;
			// look for a shader_postprocess_globals tag in the cache
			blam::tag_iterator_new(iter, TagGroups::s_shader_postprocess_globals::k_group_tag);
			// get the first instance (there should only be one, others will be ignored)
			datum_index tag_index = blam::tag_iterator_next(iter);
			if(!tag_index.IsNull())
			{
				// set the current shader globals tag reference to the tag found
				m_globals.map_postprocess_globals = TagGroups::TagGetForModify<TagGroups::s_shader_postprocess_globals>(tag_index);
			}
			else
				m_globals.map_postprocess_globals = nullptr;

			m_globals.m_flags.has_postprocess_globals = m_globals.map_postprocess_globals != nullptr;
		}

		void c_post_processing_main::ClearPostprocessGlobalsTag()
		{
			m_globals.map_postprocess_globals = nullptr;
			m_globals.m_flags.has_postprocess_globals = false;
		}

		void c_post_processing_main::OpenGlobalShaderFile()
		{
			// open the post processing shaders file for reading
			m_globals.shader_file.OpenFile("PP_PPShaders_SHD", true);
		}

		void c_post_processing_main::CloseGlobalShaderFile()
		{
			m_globals.shader_file.CloseFile();
		};

		void c_post_processing_main::UpdateStatus()
		{
			m_members.m_flags.is_ready = false;
			do
			{
				if(m_members.status != Enums::pp_component_status_initialised) break;
				if(m_members.m_flags.is_disabled) break;
				if(m_members.m_flags.is_unloaded) break;
				if(!m_globals.render_device) break;
				if(!m_globals.shader_file.IsFileMapped()) break;
				if(!m_globals.scene_buffer_chain.IsAvailable()) break;
				if(!m_globals.gbuffer) break;

				m_members.m_flags.is_ready = true;
			}while(false);
		}

		/////////////////////////////////////////////////
		// system application
		void c_post_processing_main::PollUpdate()
		{
			//TODO: add screen fov

			m_globals.m_hud.value = Hud::ScaleHUD() ? m_globals.m_hud.scaled_ratio : m_globals.m_hud.default_ratio;

			m_globals.m_clipping.near_clip = Render::RenderGlobals()->frustum.z_near;
			m_globals.m_clipping.far_clip = Render::RenderGlobals()->frustum.z_far;

			m_globals.delta_time = GameState::MainGlobals()->delta_time;

			// does the player have camera control (camera_control 0)
			m_globals.m_activation_variables.player_has_control = !Camera::DirectorScripting()->initialized;

			// are we currently in a cinematic?
			Camera::s_cinematic_globals_data* cinematic_globals = Camera::CinematicGlobals();
			m_globals.m_activation_variables.in_cinematic = (cinematic_globals ? cinematic_globals->in_progress : false);

			// are we in a vehicle?
			datum_index player_index = Players::LocalPlayerIndex();
			m_globals.m_activation_variables.local_player_in_vehicle = (!player_index.IsNull() ? Players::GetVehicle(player_index) != nullptr : false);

			// is our weapon zoomed?
			m_globals.m_activation_variables.local_player_zoomed = Players::PlayerControlGlobals()->local_players[0].zoom_level != NONE;
		}
	};};
};
#endif