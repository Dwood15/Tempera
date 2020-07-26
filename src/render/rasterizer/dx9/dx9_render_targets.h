#pragma once

#include <d3d9types.h>
#include <macros_generic.h>
#include <d3d9.h>

namespace Yelo::Rasterizer {
	struct s_render_target {
		uint      width, height;
		D3DFORMAT format;

		IDirect3DSurface9 *surface;
		IDirect3DTexture9 *texture;

		// returns whether the texture and surface are valid
		bool IsEnabled() const;

		// Create the render target texture and surface. If either of those fails [enabled] will be false
		HRESULT CreateTarget(IDirect3DDevice9 *device, uint rt_width, uint rt_height, D3DFORMAT rt_format);

		// Release the texture and surface
		void ReleaseTarget();

		// Set the device target to [surface] then clear it
		void ClearTarget(IDirect3DDevice9 *device, D3DCOLOR color = 0x00000000,
							  DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
	};

	s_render_target *GlobalRenderTargets(); // defined in the project implementation

}
