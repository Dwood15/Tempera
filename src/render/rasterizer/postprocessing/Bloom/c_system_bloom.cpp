/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

#include "Rasterizer/PostProcessing/Bloom/s_shader_bloom_definition.hpp"

#include "Rasterizer/PostProcessing/Bloom/c_settings_bloom.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_bloom.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_instance_bloom.hpp"
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/c_effect_instance.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		/////////////////////////////////////////////////
		// static members
		c_system_bloom c_system_bloom::g_bloom_system;

#include "Rasterizer/PostProcessing/Bloom/BloomDefinitions.inl"
		static TagGroups::s_shader_postprocess_globals_bloom*
												g_bloom_globals_current;
		static c_shader_bloom					g_shader_bloom;
		static c_shader_instance_bloom			g_shader_instance_bloom;
		static c_effect_postprocess				g_effect_bloom;
		static c_effect_instance				g_effect_instance;

		/////////////////////////////////////////////////
		// static member accessors
		c_system_bloom& c_system_bloom::Instance()
		{
			return g_bloom_system;
		}

		/////////////////////////////////////////////////
		// member accessors
		bool& c_system_bloom::Enabled()
		{
			return m_members.m_flags.is_enabled;
		}

		bool c_system_bloom::IsReady()
		{
			return m_members.m_flags.is_ready;
		}
		bool c_system_bloom::IsUnloaded()
		{
			return m_members.m_flags.is_unloaded;
		}

		//////////////////////////////////////////////////////////////////////
		// IPostProcessingComponent
		void c_system_bloom::Initialize()
		{
			c_settings_bloom::Register(Settings::Manager());

			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			g_shader_bloom_definition.runtime.flags.valid_shader_bit = false;
			g_shader_bloom_definition.techniques.Count = NUMBEROF(g_shader_techniques);
			g_shader_bloom_definition.techniques.Address = &g_shader_techniques[0];

			// initialise the effect definition
			g_effect_bloom_definition.quad_definition.tessellation.x = 4;
			g_effect_bloom_definition.quad_definition.tessellation.y = 4;
			g_effect_bloom_definition.quad_definition.x_bounds.lower = 0;
			g_effect_bloom_definition.quad_definition.x_bounds.upper = 1;
			g_effect_bloom_definition.quad_definition.y_bounds.lower = 0;
			g_effect_bloom_definition.quad_definition.y_bounds.upper = 1;
			g_effect_bloom_definition.runtime.flags.valid_effect_bit = false;
			g_effect_bloom_definition.runtime.flags.is_active_bit = true;

			// initialise the bloom shader
			g_shader_bloom.Ctor();
			g_shader_bloom.SetShaderDefinition(&g_shader_bloom_definition);
			g_shader_bloom.SetupShader();

			// initialise the shader instance
			g_shader_instance_bloom.Ctor();
			g_shader_instance_bloom.SetShader(&g_shader_bloom);
			g_shader_instance_bloom.SetInitialValues(g_bloom_defaults);
			g_shader_instance_bloom.SetupShaderInstance();
			g_bloom_globals_current = &g_bloom_defaults;

			// initialise the effect
			g_effect_bloom.Ctor();
			g_effect_bloom.SetEffectDefinition(&g_effect_bloom_definition);
			g_effect_bloom.AddShaderInstance(&g_shader_instance_bloom);
			g_effect_bloom.SetupEffect();

			// initialise the effect instance
			g_effect_instance.Ctor();
			g_effect_instance.SetEffect(&g_effect_bloom);
			g_effect_instance.SetQuadDefinition(&g_effect_bloom_definition.quad_definition);
			g_effect_instance.SetupEffectInstance();
		}

		void c_system_bloom::Dispose()
		{
			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			g_effect_instance.Dtor();
			g_effect_bloom.Dtor();
			g_shader_instance_bloom.Dtor();
			g_shader_bloom.Dtor();
			
			c_settings_bloom::Unregister(Settings::Manager());
		}

		void c_system_bloom::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
		{
			HRESULT hr = E_FAIL;
			// create the direct3d resources of the shader
			do
			{
				if(FAILED(hr = CreateShader())) break;
				if(FAILED(hr = CreateEffect())) break;
			}while(false);

			Validate();

			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		void c_system_bloom::OnLostDevice_Base()
		{
			// call on device lost on the shaders effect
			if(!m_members.m_flags.is_unloaded)
				SetDeviceLost();
			Validate();

			UpdateStatus();
		}

		void c_system_bloom::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
		{
			// call on device reset on the shaders effect
			HRESULT hr = E_FAIL;
			if(!m_members.m_flags.is_unloaded)
				hr = SetDeviceReset();
			Validate();

			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		void c_system_bloom::ReleaseResources_Base()
		{
			// release the direct 3d resources
			DestroyShader();
			DestroyEffect();
			Validate();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		void c_system_bloom::Unload()
		{
			// release the direct3d resources of the shader
			DestroyShader();
			DestroyEffect();
			Validate();

			m_members.status = Enums::pp_component_status_uninitialised;
			m_members.m_flags.is_unloaded = true;

			UpdateStatus();
		}

		void c_system_bloom::Load()
		{
			HRESULT hr = E_FAIL;
			// create the direct3d resources of the shader
			if(m_members.m_flags.is_unloaded)
			{
				do
				{
					if(FAILED(hr = CreateShader())) break;
					if(FAILED(hr = CreateEffect())) break;
				}while(false);

				Validate();

				if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
				else { m_members.status = Enums::pp_component_status_initialisation_failed; }

				m_members.m_flags.is_unloaded = false;
			}

			UpdateStatus();
		}

		/////////////////////////////////////////////////
		// IPostProcessingRenderable
		bool c_system_bloom::Render(Enums::postprocess_render_stage render_stage)
		{
			if(!m_members.m_flags.is_enabled)
				return false;

			bool applied = false;
			do
			{
				if(!g_bloom_defaults.flags.is_enabled_bit) break;

				// the bloom can be placed either before or after the HUD
				if(g_bloom_globals_current->flags.apply_after_hud_bit)
				{
					if(render_stage != Enums::_postprocess_render_stage_pre_ui) break;
				}
				else
				{
					if(render_stage != Enums::_postprocess_render_stage_pre_hud) break;
				}

				applied = RenderBloom(c_post_processing_main::Instance().Globals().render_device);
			}while(false);

			return applied;
		}

		/////////////////////////////////////////////////
		// IPostProcessingUpdatable
		void c_system_bloom::Update(real delta_time)
		{
			// update the standard variables in the shader, the variables in the shader instance (via the effect), and the effect instance
			g_shader_bloom.UpdateVariables();
			g_effect_bloom.Update(delta_time);
			g_effect_instance.UpdateEffectInstance(delta_time);
		}

		/////////////////////////////////////////////////
		// IPostProcessingCacheComponent
		void c_system_bloom::Initialize_Cache()
		{
			// set the shaders variables to those in the maps globals tag
			SetBloomShaderVariables();
		}

		void c_system_bloom::Dispose_Cache()
		{
			// set the shaders variables to the defaults
			SetBloomShaderVariables();
		}

		/////////////////////////////////////////////////
		// system setup
		HRESULT c_system_bloom::CreateShader()
		{
			return g_shader_bloom.LoadShader(c_post_processing_main::Instance().Globals().render_device);
		}

		void c_system_bloom::DestroyShader()
		{
			// release the shader effect
			g_shader_bloom.UnloadShader();
		}

		HRESULT c_system_bloom::CreateEffect()
		{
			// setup variables that require d3d resources to be allocated, then allocate the effect instances quad
			g_effect_bloom.SetupEffectPostCreation();
			return g_effect_instance.LoadEffectInstance();
		}

		void c_system_bloom::DestroyEffect()
		{
			// release the effect instances quad
			g_effect_instance.UnloadEffectInstance();
		}

		void c_system_bloom::SetDeviceLost()
		{
			// call OnDeviceLost on the shader
			g_shader_bloom.OnDeviceLost();
		}

		HRESULT c_system_bloom::SetDeviceReset()
		{
			// call OnDeviceReset on the shader
			return g_shader_bloom.OnDeviceReset();
		}

		void c_system_bloom::SetBloomShaderVariables()
		{
			c_post_processing_main::s_post_processing_globals& globals = c_post_processing_main::Instance().Globals();

			// get the globals block from the globals tag if present
			// if not use the defaults
			if(globals.m_flags.has_postprocess_globals && (globals.map_postprocess_globals->bloom_globals.Count == 1))
			{
				TagGroups::s_shader_postprocess_globals* postprocess_globals = globals.map_postprocess_globals;

				g_shader_instance_bloom.SetInitialValues(postprocess_globals->bloom_globals[0]);
				g_bloom_globals_current = &postprocess_globals->bloom_globals[0];
			}
			else
			{
				g_shader_instance_bloom.SetInitialValues(g_bloom_defaults);
				g_bloom_globals_current = &g_bloom_defaults;
			}
		}

		void c_system_bloom::Validate()
		{
			g_shader_bloom.Validate();
			g_shader_instance_bloom.Validate();
			g_effect_bloom.Validate();
			g_effect_instance.Validate();
		}

		void c_system_bloom::UpdateStatus()
		{
			m_members.m_flags.is_ready = false;
			do
			{
				if(m_members.status != Enums::pp_component_status_initialised) break;
				if(!g_effect_instance.IsValid()) break;

				m_members.m_flags.is_ready = true;
			}while(false);
		}

		/////////////////////////////////////////////////
		// system application
		bool c_system_bloom::RenderBloom(IDirect3DDevice9* render_device)
		{
			return SUCCEEDED(g_effect_instance.Render(render_device));
		}

		/////////////////////////////////////////////////
		// scripting
		void c_system_bloom::SetBloomSize(real size, real change_time)
		{
			g_shader_instance_bloom.SetBloomSize(size, change_time);
		}

		void c_system_bloom::SetBloomExposure(real exposure, real change_time)
		{
			g_shader_instance_bloom.SetBloomExposure(exposure, change_time);
		}

		void c_system_bloom::SetBloomMixAmount(real mix_amount, real change_time)
		{
			g_shader_instance_bloom.SetBloomMixAmount(mix_amount, change_time);
		}

		void c_system_bloom::SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time)
		{
			g_shader_instance_bloom.SetBloomMinimumColor(minimum_color, change_time);
		}

		void c_system_bloom::SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time)
		{
			g_shader_instance_bloom.SetBloomMaximumColor(maximum_color, change_time);
		}
	};};};
};
#endif