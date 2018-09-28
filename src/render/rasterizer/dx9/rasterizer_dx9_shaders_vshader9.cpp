/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"

#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_VSHADERS
#include "Memory/_EngineLayout.inl"

		s_rasterizer_dx9_effect_collection*	EffectCollections()	PTR_IMP_GET2(rasterizer_dx9_effect_collection);
		s_effect_shader_entry*	EffectShaders()					PTR_IMP_GET2(rasterizer_effect_shaders);
		s_vsf_entry*	VertexShaders()							PTR_IMP_GET2(vsf_table);
	};
};