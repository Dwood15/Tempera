#pragma once

//#define EXTERNAL_SUBSYSTEM_SHADERS

#include <d3d9.h>
#include <macros_generic.h>
#include "../dx9/shaders/shader_postprocess_definitions.hpp"

namespace Yelo::Rasterizer::PostProcessing {
	struct s_render_state_manager {
		struct s_state {
			const char *m_state_name; //TODO: debug only
			D3DRENDERSTATETYPE m_state;
			long               m_stored_value;
			long               m_set_value;
		};

		const char *m_state_set_name;
		s_state m_states[15];

		void StoreAndSet(IDirect3DDevice9 *render_device);

		void RestoreStates(IDirect3DDevice9 *render_device);
	};

	template <class T>
	struct s_component_toggle {
		bool is_ready;
		T *m_component;
	};

	template <class T>
	struct s_component_toggle_ref {
		uint32 component_index;
		T *m_component;
	};

	void Initialize();

	void Dispose();

	void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pParameters);

	void OnLostDevice();

	void OnResetDevice(D3DPRESENT_PARAMETERS *pParameters);

	void Render();

	void Release();

	void InitializeForNewMap();

	void DisposeFromOldMap();

	void Update(real delta_time);

	void RenderPreAlphaBlended();

	void RenderPreHUD();

	void RenderPreMenu();

	void RenderPostMenu();

	void LoadSystem();

	void UnloadSystem();

	void RenderSystems(Enums::postprocess_render_stage render_stage);
};
