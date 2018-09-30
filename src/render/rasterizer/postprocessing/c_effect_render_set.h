#pragma once

#include "c_effect_instance.h"

namespace Yelo::Rasterizer::PostProcessing {
	class c_effect_render_set {
		c_effect_instance **m_effect_list;

	public:
		void Ctor() {
			m_effect_list = NULL;
		}

		void Dtor() {
			delete[] m_effect_list;
			m_effect_list = NULL;
		}

		void SetCount(uint32 effect_count);

		void SetEffectInstance(uint32 index, c_effect_instance *effect);

		bool RenderSet(IDirect3DDevice9 *render_device);
	};
};
