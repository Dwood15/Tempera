#pragma once

#include <precompile.h>
#include "ui_game_data_input_functions.hpp"
#include "ui_event_handler_functions.hpp"
#include "ui_text_search_and_replace_functions.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum ui_widget_type : short
		{
			_ui_widget_type_container,
			_ui_widget_type_text_box,
			_ui_widget_type_spinner_list,
			_ui_widget_type_column_list,
			//////////////////////////////////////////////////////////////////////////
			// the following are not implemented
			_ui_widget_type_game_model,
			_ui_widget_type_movie,
			_ui_widget_type_custom,

			k_number_of_ui_widget_types,
		};
	};

	namespace TagGroups
	{
		struct ui_conditional_widget_reference
		{
			struct __flags
			{
				unsigned long load_if_event_handler_function_fails_bit:1;
			}; static_assert( sizeof(__flags) == sizeof(unsigned long) );

			tag_reference widget_tag;
			tag_string name;
			__flags flags;
			short custom_controller_index;
			
			unsigned short : 16;
			long:8 * sizeof(long) * 6;
		}; static_assert( sizeof(ui_conditional_widget_reference) == 0x50 ); // max count: 32

		struct ui_child_widget_reference
		{
			struct __flags
			{
				unsigned long use_custom_controller_index_bit:1;
			}; static_assert( sizeof(__flags) == sizeof(unsigned long) );

			tag_reference widget_tag;
			tag_string name;
			__flags flags;
			short custom_controller_index;
			short vertical_offset;
			short horizontal_offset;
			
			unsigned short : 16;
			long:8 * sizeof(long) * 5;
		}; static_assert( sizeof(ui_child_widget_reference) == 0x50 ); // max count: 32

		struct ui_widget_definition
		{
			enum { k_group_tag = 'DeLa' };

			struct __flags
			{
				unsigned long pass_unhandled_events_to_focused_child_bit:1;
				unsigned long pause_game_time_bit:1;
				unsigned long flash_background_bitmap_bit:1;
				unsigned long dpad_up_down_tabs_thru_children_bit:1;
				unsigned long dpad_left_right_tabs_thru_children_bit:1;
				unsigned long dpad_up_down_tabs_thru_list_items_bit:1;
				unsigned long dpad_left_right_tabs_thru_list_items_bit:1;
				unsigned long dont_focus_a_specific_child_widget_bit:1;
				unsigned long pass_unhandled_events_to_all_children_bit:1;
				unsigned long render_regardless_of_controller_index_bit:1;
				unsigned long pass_handled_events_to_all_children_bit:1;
				unsigned long return_to_main_menu_if_no_history_bit:1;
				unsigned long always_use_tag_controller_index_bit:1;
				unsigned long always_use_nifty_render_fx_bit:1;
				unsigned long don_t_push_history_bit:1;
				unsigned long force_handle_mouse_bit:1;
			}; static_assert( sizeof(__flags) == sizeof(unsigned long) );

			short widget_type;
			short controller_index;
			tag_string name;
			rectangle2d bounds;
			__flags flags;
			long ms_to_auto_close;
			long ms_auto_close_fade_time;
			tag_reference background_bitmap;

			////////////////////////////////////////////////////////////////
			// game data input fxns
			// These functions use current game data to modify the appearance of
			// the widget. These functions are called every time the widget is
			// rendered.
			Yelo::TagBlock<const game_data_input_reference> game_data_inputs;

			////////////////////////////////////////////////////////////////
			// event handlers
			// These allow actions to be tied to certain ui events.
			// The event handler is run every time the widget receives the specified event.
			// By default, the 'back' and 'B' buttons will take you to the previous screen.
			Yelo::TagBlock<const ui_widget_event_handler_reference> event_handlers;

			////////////////////////////////////////////////////////////////
			// search-and-replace
			// These are used to run a search-and-replace on the specified
			// word in the text-box text, replacing all occurrences of the word
			// with the output of the replace-function. These are invoked each
			// time the text box is rendered (after any game data input functions
			// have been run). The searching is case-sensitive.
			Yelo::TagBlock<const ui_widget_search_and_replace_reference> search_and_replace_functions;
			long:8 * sizeof(long) * 32;

			////////////////////////////////////////////////////////////////
			// text box
			// parameters specific to text box widgets
			// NOTE: the string list tag can also be used for lists whose items come from a string list tag
			struct __textbox {
				struct __flags
				{
					unsigned long editable_bit:1;
					unsigned long password_bit:1;
					unsigned long flashing_bit:1;
					unsigned long dont_do_that_weird_focus_test_bit:1;
				}; static_assert( sizeof(__flags) == sizeof(unsigned long) );

				tag_reference label_unicode_strings_list;
				tag_reference font;
				real_argb_color color;
				short justification;
				__flags flags;
				long:8 * sizeof(long) * 3;

				////////////////////////////////////////////////////////////////
				// more text box parameters
				short string_list_index;
				short horiz_offset;
				short vert_offset;
				short:8 * sizeof(short) * 13;

				unsigned short : 16; // because they didn't pad after justification
			}text_box;

			////////////////////////////////////////////////////////////////
			// list items
			// These options affect list items for both spinner and column lists
			// * child widgets are used to define the visible list items
			// * for lists with code-generated list items, the child widgets are used
			//   as templated for visible list item placement
			// IMPORTANT: for list widgets, the ONLY thing you can have as child widgets are the list item widgets!
			struct {
				struct __flags
				{
					unsigned long list_items_generated_in_code_bit:1;
					unsigned long list_items_from_string_list_tag_bit:1;
					unsigned long list_items_only_one_tooltip_bit:1;
					unsigned long list_single_preview_no_scroll_bit:1;
				}; static_assert( sizeof(__flags) == sizeof(unsigned long) );

				__flags flags;

				////////////////////////////////////////////////////////////////
				// spinner list
				// parameters specific to spinner list widgets
				// child widgets are the list items
				struct {
					tag_reference list_header_bitmap;
					tag_reference list_footer_bitmap;
					rectangle2d header_bounds;
					rectangle2d footer_bounds;
					long:8 * sizeof(long) * 8;
				}spinner;

				////////////////////////////////////////////////////////////////
				// column list
				// parameters specific to column list widgets
				// child widgets are the list items
				struct {
					tag_reference extended_description_widget;
					long:8 * sizeof(long) * 8;
					long:8 * sizeof(long) * 64;
				}column;
			}list_items;

			////////////////////////////////////////////////////////////////
			// conditional widgets
			// use this to attach widgets that are loaded only
			// if some internal criteria is met while processing a widget event
			Yelo::TagBlock<const ui_conditional_widget_reference> conditional_widgets;
			long:8 * sizeof(long) * 32;
			long:8 * sizeof(long) * 32;

			////////////////////////////////////////////////////////////////
			// child widgets
			// use this to attach widgets that are loaded as 'children'
			// of this widget (children are always loaded as part of the parent widget)
			Yelo::TagBlock<const ui_child_widget_reference> child_widgets;
		}; static_assert( sizeof(ui_widget_definition) == 0x3EC );
	};

	namespace blam
	{
		void __cdecl ui_load_tags_for_scenario(datum_index scenario_index);
	};
};
