/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

#include "Rasterizer/PostProcessing/FXAA/c_settings_fxaa.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_shader_fxaa.hpp"
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/c_effect_instance.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace FXAA
	{
		/////////////////////////////////////////////////
		// static members
		c_system_fxaa c_system_fxaa::g_fxaa_system;

#include "Rasterizer/PostProcessing/FXAA/FXAADefinitions.inl"

		static c_shader_fxaa				g_shader_fxaa;
		static c_shader_instance			g_shader_instance_fxaa;
		static c_effect_postprocess			g_effect_fxaa;
		static c_effect_instance			g_effect_instance_fxaa;

		/////////////////////////////////////////////////
		// static member accessors
		c_system_fxaa& c_system_fxaa::Instance()
		{
			return g_fxaa_system;
		}

		/////////////////////////////////////////////////
		// member accessors
		bool& c_system_fxaa::Enabled()
		{
			return m_members.m_flags.is_enabled;
		}

		bool c_system_fxaa::IsReady()
		{
			return m_members.m_flags.is_ready;
		}

		bool c_system_fxaa::IsUnloaded()
		{
			return m_members.m_flags.is_unloaded;
		}

		//////////////////////////////////////////////////////////////////////
		// IPostProcessingComponent
		void c_system_fxaa::Initialize()
		{
			c_settings_fxaa::Register(Settings::Manager());

			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			g_shader_fxaa_definition.runtime.flags.valid_shader_bit = false;
			g_shader_fxaa_definition.techniques.Count = NUMBEROF(g_shader_techniques);
			g_shader_fxaa_definition.techniques.Address = &g_shader_techniques[0];

			// initialise the effect definition
			g_effect_fxaa_definition.quad_definition.tessellation.x = 4;
			g_effect_fxaa_definition.quad_definition.tessellation.y = 4;
			g_effect_fxaa_definition.quad_definition.x_bounds.lower = 0;
			g_effect_fxaa_definition.quad_definition.x_bounds.upper = 1;
			g_effect_fxaa_definition.quad_definition.y_bounds.lower = 0;
			g_effect_fxaa_definition.quad_definition.y_bounds.upper = 1;
			g_effect_fxaa_definition.runtime.flags.valid_effect_bit = false;
			g_effect_fxaa_definition.runtime.flags.is_active_bit = true;

			// initialise the fxaa shader
			g_shader_fxaa.Ctor();
			g_shader_fxaa.SetShaderDefinition(&g_shader_fxaa_definition);
			g_shader_fxaa.SetupShader();

			// initialise the shader instance
			g_shader_instance_fxaa.Ctor();
			g_shader_instance_fxaa.SetShader(&g_shader_fxaa);
			g_shader_instance_fxaa.SetupShaderInstance();

			// initialise the effect
			g_effect_fxaa.Ctor();
			g_effect_fxaa.SetEffectDefinition(&g_effect_fxaa_definition);
			g_effect_fxaa.AddShaderInstance(&g_shader_instance_fxaa);
			g_effect_fxaa.SetupEffect();

			// initialise the effect instance
			g_effect_instance_fxaa.Ctor();
			g_effect_instance_fxaa.SetEffect(&g_effect_fxaa);
			g_effect_instance_fxaa.SetQuadDefinition(&g_effect_fxaa_definition.quad_definition);
			g_effect_instance_fxaa.SetupEffectInstance();
		}

		void c_system_fxaa::Dispose()
		{
			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			g_effect_instance_fxaa.Dtor();
			g_effect_fxaa.Dtor();
			g_shader_instance_fxaa.Dtor();
			g_shader_fxaa.Dtor();

			c_settings_fxaa::Unregister(Settings::Manager());
		}

		void c_system_fxaa::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
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

		void c_system_fxaa::OnLostDevice_Base()
		{
			// call on device lost on the shaders effect
			if(!m_members.m_flags.is_unloaded)
				SetDeviceLost();
			Validate();

			UpdateStatus();
		}

		void c_system_fxaa::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
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

		void c_system_fxaa::ReleaseResources_Base()
		{
			// release the direct 3d resources
			DestroyShader();
			DestroyEffect();
			Validate();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		void c_system_fxaa::Unload()
		{
			// release the direct3d resources of the shader
			DestroyShader();
			DestroyEffect();
			Validate();

			m_members.status = Enums::pp_component_status_uninitialised;
			m_members.m_flags.is_unloaded = true;

			UpdateStatus();
		}

		void c_system_fxaa::Load()
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
		bool c_system_fxaa::Render(Enums::postprocess_render_stage render_stage)
		{
			if(!m_members.m_flags.is_enabled || (render_stage != Enums::_postprocess_render_stage_pre_hud))
				return false;

			return Render(c_post_processing_main::Instance().Globals().render_device);
		}

		/////////////////////////////////////////////////
		// system setup
		HRESULT c_system_fxaa::CreateShader()
		{
			return g_shader_fxaa.LoadShader(c_post_processing_main::Instance().Globals().render_device);
		}

		void c_system_fxaa::DestroyShader()
		{
			// release the shader effect
			g_shader_fxaa.UnloadShader();
		}

		HRESULT c_system_fxaa::CreateEffect()
		{
			g_effect_fxaa.SetupEffectPostCreation();
			return g_effect_instance_fxaa.LoadEffectInstance();
		}

		void c_system_fxaa::DestroyEffect()
		{
			// release the shader effect
			g_effect_instance_fxaa.UnloadEffectInstance();
		}

		void c_system_fxaa::SetDeviceLost()
		{
			// call OnDeviceLost on the shader effect
			g_shader_fxaa.OnDeviceLost();
		}

		HRESULT c_system_fxaa::SetDeviceReset()
		{
			// call OnDeviceReset on the shader effect
			return g_shader_fxaa.OnDeviceReset();
		}

		void c_system_fxaa::Validate()
		{
			g_shader_fxaa.Validate();
			g_shader_instance_fxaa.Validate();
			g_effect_fxaa.Validate();
			g_effect_instance_fxaa.Validate();
		}

		void c_system_fxaa::UpdateStatus()
		{
			m_members.m_flags.is_ready = false;
			do
			{
				if(m_members.status != Enums::pp_component_status_initialised) break;
				if(!g_effect_instance_fxaa.IsValid()) break;

				m_members.m_flags.is_ready = true;
			}while(false);
		}

		/////////////////////////////////////////////////
		// system application
		bool c_system_fxaa::Render(IDirect3DDevice9* render_device)
		{
			return SUCCEEDED(g_effect_instance_fxaa.Render(render_device));
		}
	};};};
};
#endif