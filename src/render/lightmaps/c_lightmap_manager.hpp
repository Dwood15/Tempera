#pragma once

#include <d3d9.h>
#include "../../memory/datum_index.h"
#include "../../bitmaps/bitmap_group.h"

namespace Yelo::Render::Lightmaps {
	class c_lightmap_manager final {
	public:
		enum lightmap_type_flags : short {
			_lightmap_type_flags_standard_bit    = 1,
			_lightmap_type_flags_directional_bit = 2,
		};

	private:
		lightmap_type_flags m_available_lightmaps;

		struct s_lightmap_tag_indices {
			datum_index standard_tag_index;               //!< Tag index for the current standard lightmap.
			datum_index directional_tag_indices[3];         //!< Tag indices for the current directional lightmaps.
		}                   m_current_lightmaps;

	public:
		typedef TagGroups::s_bitmap_data *(*proc_get_bitmap_data)(const datum_index, const int32);

		bool HasLightmaps(const lightmap_type_flags flag) const;

		void SetLightmapDatums(const datum_index standard_tag_index, const datum_index directional_1_tag_index,
									  const datum_index directional_2_tag_index, const datum_index directional_3_tag_index);

		void SetLightmapSamplers(LPDIRECT3DDEVICE9 device, const int32 lightmap_index, const bool use_directional,
										 const proc_get_bitmap_data get_bitmap_data);

		datum_index GetStandardLightmapTagIndex();
	};
};
