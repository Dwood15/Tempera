/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <d3d9.h>
#include "c_lightmap_manager.hpp"
#include "../../memory/datum_index.h"
#include "../../RuntimeManager.h"

namespace Yelo {
	namespace Render {
		namespace Lightmaps {
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Returns true if the lightmap set has certain lightmap types. </summary>
			///
			/// <param name="flag">	The lightmap type. </param>
			///
			/// <returns>	true if the requested type is available, false if not. </returns>
			bool c_lightmap_manager::HasLightmaps(const lightmap_type_flags flag) const {
				return (m_available_lightmaps & flag) == flag;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Changes the active lightmap tag indices. </summary>
			///
			/// <param name="standard_tag_index">	  	The standard lightmaps tag index. </param>
			/// <param name="directional_1_tag_index">	The first directional lightmaps tag index. </param>
			/// <param name="directional_2_tag_index">	The second directional lightmaps tag index. </param>
			/// <param name="directional_3_tag_index">	The third directional lightmaps tag index. </param>
			void c_lightmap_manager::SetLightmapDatums(const datum_index standard_tag_index, const datum_index directional_1_tag_index,
																	 const datum_index directional_2_tag_index, const datum_index directional_3_tag_index) {
				m_available_lightmaps = (lightmap_type_flags) 0;

				// Update the current lightmap bitmap tag indices
				m_current_lightmaps.standard_tag_index = standard_tag_index;

				m_current_lightmaps.directional_tag_indices[0] = directional_1_tag_index;
				m_current_lightmaps.directional_tag_indices[1] = directional_2_tag_index;
				m_current_lightmaps.directional_tag_indices[2] = directional_3_tag_index;

				if (!m_current_lightmaps.standard_tag_index.IsNull()) {
					m_available_lightmaps = (lightmap_type_flags) (m_available_lightmaps | _lightmap_type_flags_standard_bit);
				}

				// We shouldn't have a case where only the first DLM is present, map should not compile without all 3
				if (!m_current_lightmaps.directional_tag_indices[0].IsNull()) {
					m_available_lightmaps = (lightmap_type_flags) (m_available_lightmaps | _lightmap_type_flags_directional_bit);
				}
			}


			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the lightmap samplers. </summary>
			///
			/// <param name="device">		  	The current render device. </param>
			/// <param name="lightmap_index"> 	Index of the lightmap. </param>
			/// <param name="get_bitmap_data">	Function for getting a bitmap data block. </param>
			void c_lightmap_manager::SetLightmapSamplers(LPDIRECT3DDEVICE9 device, const int32 lightmap_index, const bool use_directional,
																		const proc_get_bitmap_data get_bitmap_data) {
				auto set_sampler = [&](datum_index bitmap_tag_index, int32 stage) {
					auto bitmap_data = get_bitmap_data(bitmap_tag_index, lightmap_index);

					auto got = CurrentRuntime->getFunctionBegin("rasterizer_set_texture_bitmap_data");

					if (got) {
						__asm mov esi, bitmap_data
						::calls::DoCall<Convention::m_cdecl, bool, short>(*got, stage);
					}

					device->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
					device->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
					device->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					device->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					device->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
				};

				if (HasLightmaps(_lightmap_type_flags_directional_bit) && use_directional) {
					// The map does have directional lightmaps so use those
					set_sampler(m_current_lightmaps.directional_tag_indices[0], 2);
					set_sampler(m_current_lightmaps.directional_tag_indices[1], 4);
					set_sampler(m_current_lightmaps.directional_tag_indices[2], 5);
				} else {
					// The map does not have directional lightmaps, so use the standard lightmaps
					set_sampler(m_current_lightmaps.standard_tag_index, 2);
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets standard lightmap tag index. </summary>
			///
			/// <returns>	The standard lightmap tag index. </returns>
			datum_index c_lightmap_manager::GetStandardLightmapTagIndex() {
				return m_current_lightmaps.standard_tag_index;
			}
		};
	};
};
