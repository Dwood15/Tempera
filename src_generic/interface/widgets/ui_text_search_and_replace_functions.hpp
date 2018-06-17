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
		enum ui_widget_replace_function : short
		{
			_ui_widget_replace_function_null,
			_ui_widget_replace_function_widget_s_controller,
			_ui_widget_replace_function_build_number,
			_ui_widget_replace_function_pid,

			k_number_of_ui_widget_replace_functions,
		};
	};

	namespace TagGroups
	{
		struct ui_widget_search_and_replace_reference
		{
			TAG_FIELD(tag_string, search_string);
			TAG_ENUM(replace_function, Enums::ui_widget_replace_function);
		}; static_assert( sizeof(ui_widget_search_and_replace_reference) == 0x22 ); // max count: 32
	};
};
