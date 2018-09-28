#pragma once

#include <d3d9.h>
#include <d3d9types.h>

namespace Yelo::Rasterizer::PostProcessing {
			struct IPostProcessingComponent {
			virtual void Initialize() {}
			virtual void Dispose() {}

			virtual void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters) {}
			virtual void OnLostDevice_Base() {}
			virtual void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters) {}
			virtual void ReleaseResources_Base() {}

			virtual bool IsReady() { return false; }
			virtual bool IsUnloaded() { return true; }

			virtual void Unload() {}
			virtual void Load() {}
		};
	};

