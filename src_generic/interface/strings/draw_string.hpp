/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_tab_stops = 16,
		};

		enum text_style : short
		{
			_text_style_plain = NONE,

			_text_style_bold = 0,
			_text_style_italic,
			_text_style_condense,
			_text_style_underline,
		};

		enum text_justification : short
		{
			_text_justification_left,
			_text_justification_right,
			_text_justification_center,
		};
	};

	namespace Flags
	{
		enum text_flag : long_flags
		{
			_text_bit1,
			_text_bit2,
			_text_bit4,
			_text_bit8,
		};
	};

	namespace GameUI
	{
		struct s_text_bounds_globals_data
		{
			rectangle2d bounds;
			void* styled_font_tag; // 'font'
		};
		s_text_bounds_globals_data*	TextBoundsGlobals();

		struct s_font_drawing_globals_data
		{
			datum_index internal_string_localization_tag;
			datum_index font_tag;
			Flags::text_flag flags;
			Enums::text_style style;
			Enums::text_justification justification;
			real_argb_color color;
			short tab_stop_count;
			wchar_t tab_stops[Enums::k_maximum_number_of_tab_stops];
			short highlight_start;
			short highlight_end;
			short initial_indent;
			short paragraph_indent[72];
		};
		s_font_drawing_globals_data*	FontDrawingGlobals();
	};
};
