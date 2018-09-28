/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/RenderTargetChain.hpp"
#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace DX9
	{
		//////////////////////////////////////////////////////////////////////////
		// s_render_target_chain
		bool		s_render_target_chain::IsAvailable() const
		{
			return m_targets[0].IsEnabled() &&
				m_targets[1].IsEnabled();
		}

		bool		s_render_target_chain::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height, D3DFORMAT format)
		{
			HRESULT success = 0;
			for(int i = 0; i < 2; i++)
				success |= m_targets[i].CreateTarget(device,
					width,
					height,
					format);

			ResetTargets();

			return SUCCEEDED(success);
		}

		void		s_render_target_chain::ResetTargets()
		{
			m_target_setup.current = &m_targets[0];
			m_target_setup.next = &m_targets[1];
		}

		void		s_render_target_chain::ReleaseResources()
		{
			for(int i = 0; i < 2; i++)
				m_targets[i].ReleaseTarget();
		}

		void		s_render_target_chain::Flip()
		{
			std::swap(m_target_setup.current, m_target_setup.next);
		}

		//////////////////////////////////////////////////////////////////////////
		// s_render_target_chain_scene
		bool		s_render_target_chain_scene::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			bool success = s_render_target_chain::AllocateResources(
				device, 
				width, height, 
				Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].format);

			return success;
		}

		void		s_render_target_chain_scene::ResetTargets()
		{
			m_target_setup_scene.scene = &Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];

			s_render_target_chain::ResetTargets();
		}

		void		s_render_target_chain_scene::SetSceneToLast()
		{
			std::swap(m_target_setup.next, m_target_setup_scene.scene);
		}
	};
};
#endif