/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/Rasterizer.hpp"

namespace Yelo
{
	namespace DX9
	{
		/////////////////////////////////////////////////////////////////////
		// Used to prevent reading and writing to a single render target by
		// flipping between two on each pass.
		/////////////////////////////////////////////////////////////////////
		struct s_render_target_chain
		{
			bool m_first_render;
			PAD24;
			// 2 render targets
			Render::s_render_target m_targets[2];
			// pointers to the current setup of targets
			struct{
				// current holds the target currently being rendered to
				Render::s_render_target* current;
				// next hold the next (also last) target rendered to
				Render::s_render_target* next;
			}m_target_setup;

			// returns true if both of the extra render targets are ok to use
			bool				IsAvailable() const;
			// create the render targets
			virtual bool		AllocateResources(IDirect3DDevice9* device,
				uint32 width, 
				uint32 height,
				D3DFORMAT format);
			// resets the target setup
			virtual void		ResetTargets();
			// release the render targets
			void				ReleaseResources();
			// swaps the pointers for current and next
			void				Flip();


			IDirect3DSurface9*  GetCurrentSurface()	const	{ return m_target_setup.current->surface; }
			IDirect3DTexture9*  GetCurrentTexture()	const	{ return m_target_setup.current->texture; }
			IDirect3DSurface9*  GetNextSurface()	const	{ return m_target_setup.next->surface; }
			IDirect3DTexture9*  GetNextTexture()	const	{ return m_target_setup.next->texture; }
		};

		struct s_render_target_chain_scene : public s_render_target_chain
		{
			struct{
				// scene holds the scene as it was before an effect is started
				Render::s_render_target* scene;
			}m_target_setup_scene;

			// create the render targets
			bool				AllocateResources(IDirect3DDevice9* device,
				uint32 width, 
				uint32 height);
			// resets the target setup to the default of halos primary buffer as the scene texture
			void				ResetTargets();
			// swaps the pointers for scene and next
			void				SetSceneToLast();

			IDirect3DSurface9*  GetSceneSurface()	const	{ return m_target_setup_scene.scene->surface; }
			IDirect3DTexture9*  GetSceneTexture()	const	{ return m_target_setup_scene.scene->texture; }
		};
	};
};
#endif