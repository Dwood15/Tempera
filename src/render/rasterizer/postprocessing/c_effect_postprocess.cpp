/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/i_interpolator.hpp>
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/Fade/c_system_fade.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_effect_postprocess::SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition)
		{
			m_members.effect_definition = definition;
		}

		bool c_effect_postprocess::IsValid()
		{
			return m_members.effect_definition->runtime.flags.valid_effect_bit;
		}

		/////////////////////////////////////////////////
		// effect setup
		void c_effect_postprocess::SetupEffectPostCreation()
		{
			// determine whether this effect uses the gbuffer at any point
			c_shader_instance* current = m_members.m_shaders.list;
			while(current)
			{
				m_members.effect_definition->runtime.flags.uses_gbuffer_bit |= current->UsesGBuffer();
				current = current->GetNext();
			}
		}

		/*!
		 * \brief
		 * Adds a shader instance to the effects shader instance list.
		 * 
		 * \param instance
		 * A pointer to the shader instance being added to the effect.
		 * 
		 * Adds a shader instance to the effects shader instance list. The effect does not handle memory allocated for the shader instances
		 * So they must be handled elsewhere.
		 */
		void c_effect_postprocess::AddShaderInstance(c_shader_instance* instance)
		{
			Yelo::AppendLinkedListNode(m_members.m_shaders.list, instance);
			m_members.m_shaders.count++;
		}

		void c_effect_postprocess::Validate()
		{
			m_members.effect_definition->runtime.flags.valid_effect_bit = ValidateImpl();
		}

		bool c_effect_postprocess::ValidateImpl()
		{
			bool valid = false;
			do
			{
				if(!m_members.effect_definition) break;
				if(!m_members.m_shaders.list) break;
				if(m_members.m_shaders.count == 0) break;

				valid = true;
			}while(false);
			
			// the effect is invalid if one of it's shader instances is invalid
			c_shader_instance* current = m_members.m_shaders.list;
			while(current && valid)
			{
				current->Validate();

				valid &= current->IsValid();
				current = current->GetNext();
			}

			return valid;
		}

		/////////////////////////////////////////////////
		// effect application
		/*!
		 * \brief
		 * Updates all of the effects shader instances.
		 * 
		 * \param delta_time
		 * The time that has passed since the last update.
		 * 
		 * Updates all of the effects shader instances.
		 */
		void c_effect_postprocess::Update(real delta_time)
		{
			c_shader_instance* current = m_members.m_shaders.list;
			while(current)
			{
				current->UpdateShaderInstance(delta_time);
				current = current->GetNext();
			}
		}

		/*!
		 * \brief
		 * Renders the effects shaders in order.
		 * 
		 * \param render_device
		 * The current D3D render device.
		 * 
		 * \param render_quad
		 * The quad instance to render the effect with.
		 * 
		 * \returns
		 * Non-zero if an error occured.
		 * 
		 * Renders the effects shaders in order. Each shader instance in the shader list will be instructed to set its
		 * variables to the base shaders effect before being drawn using the supplied render device.
		 */
		HRESULT c_effect_postprocess::Render(IDirect3DDevice9* render_device, c_quad_instance* render_quad)
		{
			return Render(render_device, render_quad, 1.0f);
		}

		/*!
		 * \brief
		 * Renders the effects shaders in order.
		 * 
		 * \param render_device
		 * The current D3D render device.
		 * 
		 * \param render_quad
		 * The quad instance to render the effect with.
		 * 
		 * \param fade_amount
		 * The amount to fade the effects result by (0.0f to 1.0f).
		 * 
		 * \returns
		 * Non-zero if an error occured.
		 * 
		 * Renders the effects shaders in order. Each shader instance in the shader list will be instructed to set its
		 * variables to the base shaders effect before being drawn using the supplied render device.
		 */
		HRESULT c_effect_postprocess::Render(IDirect3DDevice9* render_device, c_quad_instance* render_quad, real fade_amount)
		{
			// if the effect is invalid, return
			if(!IsActive())
				return E_FAIL;

			// fade amount is zero so do not render the effect
			if(fade_amount == 0.0f)
				return E_FAIL;

			// set the scene render chain so that the source and scene textures are the same at the start of the effect
			c_post_processing_main::Instance().Globals().scene_buffer_chain.m_first_render = true;
			c_post_processing_main::Instance().Globals().secondary_buffer_chain.m_first_render = true;

			HRESULT hr = E_FAIL;
			// apply each shader in order
			c_shader_instance* current = m_members.m_shaders.list;
			while(current)
			{
				if(current->IsActive())
				{
					// set the shaders values to those in the instance
					current->SetShaderInstanceVariables();
					// render the shader using the effects quad instance
					hr = current->GetShader()->Render(render_device, render_quad);

					if(FAILED(hr)) break;
				}
				current = current->GetNext();
			}

			// if the fade amount is less than 1.0f, fade the effects result
			if(fade_amount < 1.0f)
				hr |= Fade::c_system_fade::Instance().FadeCurrentResult(fade_amount);

			// if errors occurred leave the render-target as-is, otherwise swap the targets so that the result of 
			// this effect becomes the scene texture for the next
			if (SUCCEEDED(hr))
				c_post_processing_main::Instance().Globals().scene_buffer_chain.SetSceneToLast();
			return hr;
		}

		bool c_effect_postprocess::IsActive()
		{
			bool active = m_members.effect_definition->runtime.flags.valid_effect_bit;
			if(m_members.effect_definition->runtime.flags.uses_gbuffer_bit && !DX9::c_gbuffer_system::g_system_enabled)
				active = false;
			return active;
		}
	}; };
};
#endif