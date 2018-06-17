/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		typedef bool (__cdecl* proc_widget_type)(datum_index object_datum);
		struct s_widget_type_definition
		{
			tag group_tag;
			bool needs_lighting; unsigned char : 8; unsigned short : 16;
			proc_initialize				initialize_proc;
			proc_initialize_for_new_map initialize_for_new_map_proc;
			proc_dispose_from_old_map	dispose_from_old_map_proc;
			proc_dispose				dispose_proc;
			proc_widget_type			new_proc;
			proc_widget_type 			delete_proc;
			proc_widget_type 			update_proc;
			proc_widget_type 			render_proc;
		};

		struct s_widget_datum : Memory::s_datum_base
		{
			short type;
			datum_index type_datum_index; // index to the type's data array (eg, antenna)
			datum_index next_widget_index;
		}; static_assert( sizeof(s_widget_datum) == 0xC );
	};
};
