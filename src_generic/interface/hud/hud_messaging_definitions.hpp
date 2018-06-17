/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_max_state_messages = 1024,
			k_max_state_text_panels = 8,
		};

		enum e_hud_message_type : byte_enum {
			_hud_message_type_text,
			_hud_message_type_icon,

			k_number_of_hud_message_types
		};
	};

	namespace TagGroups
	{
		struct hud_state_message_element
		{
			byte_enum type;
			sbyte data;
		}; static_assert( sizeof(hud_state_message_element) == 0x2 );

		struct hud_state_message_definition
		{
			tag_string name;
			short start_index_into_text_blob;
			short start_index_of_message_block;
			sbyte panel_count; unsigned char : 8; unsigned short : 16;
			long:8 * sizeof(long) * 6;
		}; static_assert( sizeof(hud_state_message_definition) == 0x40 );

		struct hud_state_messages
		{
			enum { k_group_tag = 'hmt ' };

			tag_data text_data;
			Yelo::TagBlock<const hud_state_message_element> message_elements;
			Yelo::TagBlock<const hud_state_message_definition> messages;
			long:8 * sizeof(long) * 21;
		}; static_assert( sizeof(hud_state_messages) == 0x80 );
	};
};
