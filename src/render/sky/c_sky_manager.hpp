/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo
{
	namespace Render { namespace Sky
	{
		class c_sky_manager final
		{
			class s_sky_entry final
			{
				datum_index m_sky_tag_index;
				bool m_is_override;
				PAD24;
				std::shared_ptr<s_sky_entry> m_original_sky_entry;

			public:
				s_sky_entry(const datum_index sky_tag_index);
				s_sky_entry(const datum_index sky_tag_index, std::shared_ptr<s_sky_entry> sky_entry);

				bool IsOverride() const;

				datum_index GetTagIndex() const;

				std::shared_ptr<s_sky_entry> OriginalSky() const;
			};
			typedef std::shared_ptr<s_sky_entry> sky_entry_ptr_t;
			typedef std::vector<sky_entry_ptr_t> sky_entry_list_t;

			sky_entry_list_t m_sky_list;

		public:
			void Reset();
			void Clear();

			void SetScenarioSkies(const TagBlock<tag_reference>& skies);
			void SetSkyIndex(const byte sky_index, const datum_index sky_tag_index);
			datum_index GetSkyTagIndex(const byte sky_index) const;
		};
	};};
};