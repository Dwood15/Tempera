/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_effect_instance.hpp"

#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>
#include <YeloLib/Halo1/time/interpolation/i_interpolator.hpp>

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_effect_instance::SetEffect(c_effect_postprocess* definition)
		{
			m_members.definition = definition;
		}

		void c_effect_instance::SetQuadDefinition(TagGroups::s_effect_postprocess_quad_definition* definition)
		{
			m_members.quad_definition = definition;
		}

		bool c_effect_instance::IsValid()
		{
			return m_members.m_flags.is_valid;
		}

		real c_effect_instance::GetCurrentFade()
		{
			return m_members.m_fade.current;
		}

		int16 c_effect_instance::GetFadeDirection()
		{
			if(m_members.m_fade.current == m_members.m_fade.end)
				return -1;

			return m_members.m_fade.start < m_members.m_fade.end ? 1 : 0;
		}

		void c_effect_instance::SetIsActive(bool active)
		{
			m_members.m_flags.is_active = active;
		}

		/////////////////////////////////////////////////
		// effect instance setup
		/*!
		 * \brief
		 * Sets the is_valid flag.
		 * 
		 * Sets the is_valid flag.
		 */
		void c_effect_instance::Validate()
		{
			m_members.m_flags.is_valid = ValidateImpl();
		}

		HRESULT c_effect_instance::LoadEffectInstance()
		{
			YELO_ASSERT_DISPLAY(m_members.quad_definition != nullptr, "no quad definition has been set for an effect instance");

			if(m_members.quad_definition)
				m_members.render_quad = c_quad_manager::Instance().CreateQuad(*m_members.quad_definition);

			return (m_members.render_quad ? S_OK : E_FAIL);
		}

		void c_effect_instance::UnloadEffectInstance()
		{
			safe_release<c_quad_instance>(m_members.render_quad);
		}

		/*!
		 * \brief
		 * Returns whether this effect instance is valid.
		 * 
		 * \returns
		 * True if the effect instance is valid.
		 * 
		 * Returns whether this effect instance is valid. An effect instance is valid if it points to a valid effect.
		 */
		bool c_effect_instance::ValidateImpl()
		{
			bool valid = false;
			do
			{
				if(!m_members.definition) break;
				if(!m_members.render_quad) break;

				valid = true;
			}while(false);

			if(valid)
				valid &= m_members.definition->IsValid();
			return valid;
		}

		/////////////////////////////////////////////////
		// effect instance application
		bool c_effect_instance::IsActive()
		{
			return m_members.m_flags.is_active;
		}

		HRESULT c_effect_instance::Render(IDirect3DDevice9* render_device)
		{
			if(!IsActive())
				return E_FAIL;

			return m_members.definition->Render(render_device, m_members.render_quad, m_members.m_fade.current);
		}

		void c_effect_instance::UpdateEffectInstance(real delta_time)
		{
			m_members.m_fade.interpolator.Update(delta_time);

			Time::Interpolation::InterpolateValues<1>(&m_members.m_fade.start, &m_members.m_fade.end, m_members.m_fade.interpolator.GetValues(), &m_members.m_fade.current);
		}

		void c_effect_instance::SetEffectFade(real start, real end, real change_time)
		{
			m_members.m_fade.start = start;
			m_members.m_fade.end = end;
			m_members.m_fade.current = start;

			m_members.m_fade.interpolator.Begin(change_time);
		}
	};};
};
#endif