/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/Generic/c_effect_generic.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace External
	{
		class c_effect_external : public c_effect_generic
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				tag_string effect_id;
			}m_members_external;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetEffectID(const char* id);
			const char* GetEffectID();

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_effect_generic::Ctor();

				m_members_external.effect_id[0] = '\0';
			}

			void Dtor()
			{
				c_effect_generic::Dtor();

				m_members_external.effect_id[0] = '\0';
			}
		};
	};};};};
};
#endif