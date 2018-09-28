/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Fade/c_shader_instance_fade.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Fade
	{
		/*!
		 * \brief
		 * Sets the fade shader this class will instance.
		 * 
		 * \param definition
		 * The fade shader this class will instance.
		 * 
		 * Sets the fade shader this class will instance.
		 * 
		 * \see
		 * c_shader_fade
		 */
		void c_shader_instance_fade::SetShader(c_shader_postprocess* definition)
		{
			m_members_fade.definition = CAST_PTR(c_shader_fade*, definition);
			c_shader_instance::SetShader(definition);
		}

		/*!
		 * \brief
		 * Sets the shaders parameters to the current fade values.
		 * 
		 * Sets the shaders parameters to the current fade values.
		 */
		void c_shader_instance_fade::SetShaderInstanceVariables()
		{
			s_shader_fade_definition* definition = m_members.definition->GetShaderDefinition<s_shader_fade_definition>();
			YELO_ASSERT_DISPLAY(definition != nullptr, "Fade shader has no tag definition");

			LPD3DXEFFECT effect = m_members.definition->GetEffect();
			// TODO: why are we reasserting definition?
			YELO_ASSERT_DISPLAY(definition != nullptr, "Fade shader has no valid effect");

			definition->fade_amount_handle.SetVariable(effect, &m_members_fade.fade_amount, false);
		};

		/*!
		 * \brief
		 * Custom render function for fading an effects result.
		 * 
		 * \param render_device
		 * The current render device.
		 * 
		 * \param quad_instance
		 * The quad instance to render with.
		 * 
		 * \param fade_amount
		 * The amount to fade the result by.
		 * 
		 * \returns
		 * S_OK if successful, non-zero if otherwise.
		 * 
		 * Custom render function for fading an effects result. The fade effect swaps the current target with the scene texture, and re draws 
		 * it with alpha blending to fade the result in/out.
		 */
		HRESULT c_shader_instance_fade::Render(IDirect3DDevice9* render_device, c_quad_instance* quad_instance, real fade_amount)
		{
			m_members_fade.fade_amount = fade_amount;

			// set the effect result as the scene texture
			c_post_processing_main::Instance().Globals().scene_buffer_chain.SetSceneToLast();
			// set the scene prior to the effect as the render target
			c_post_processing_main::Instance().Globals().scene_buffer_chain.Flip();

			// set the fade value to the shader
			SetShaderInstanceVariables();

			// set the render state to enable alpha blending
			render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// render the effect
			HRESULT hr = GetShader()->Render(render_device, quad_instance);

			// reset the render state to disable alpha blending
			render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
			render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

			return hr;
		}
	}; }; };
};
#endif