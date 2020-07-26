#pragma once


// Terminology:
// maps_path: The system directory path to a "maps\" folder.
//				Generally one should be under the CWD, or the user profile. Dedicated server's default behavior is to
//				query the registry for the client's "EXE Path" before it checks the CWD.
// map_path: The system file path to a given cache file
// map_name: The name of a cache file without path information or extensions

#include <synchapi.h>
#include "macros_generic.h"
#include "../memory/datum_index.h"
#include "../tags/group/tag_groups.h"
#include "cache_files_structures.h"
#include "cache_files_globals.h"

namespace Yelo {
	namespace Cache {
		struct s_cache_file_request_params {
			typedef void (__cdecl *proc_finished)(void *params);

			bool          *finished_flag;// NOT optional
			void          *finished_proc_params;   // optional
			proc_finished finished_proc;// optional
		};

		struct s_cache_file_request {
			OVERLAPPED overlapped;
			uint       size;
			void       *address;
			bool       block;
			bool       reading;
			bool       running;
			unsigned char : 8;
			::Yelo::Enums::cache_file_request_source source;
			unsigned char               : 8;
			unsigned short              : 16;
			Yelo::Cache::s_cache_file_request_params params;
		}; static_assert(sizeof(s_cache_file_request) == 0x30);

		static_assert(sizeof(s_cache_file_request_params) == 0xC);


		static const char *  DataFileTypeToString(Enums::data_file_type type) {
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wswitch"
			switch (type) {
				case Enums::_data_file_type_bitmaps:
					return "bitmaps";
				case Enums::_data_file_type_sounds:
					return "sounds";
				case Enums::_data_file_type_locale:
					return "loc";
			}
//#pragma clang diagnostic pop
			throw "What type of DataFile are you looking for???";
		}

		static const char *  DataFileTypeToString(Enums::data_file_reference_type type) {
			return DataFileTypeToString((Enums::data_file_type)(type - Enums::_data_file_reference_bitmaps));
		}

	};

	namespace blam {
		bool __cdecl cache_file_read_request(/*datum_index tag_index,*/ // unused, and optimized out, at runtime
			uint offset, uint size, void *buffer, const Cache::s_cache_file_request_params &params,
			bool block = true, Yelo::Enums::cache_file_request_source source = Yelo::Enums::_cache_file_request_source_open_map);

		// made up names; H1 didn't have a "geometry_cache" like it does for textures and sounds
		// void __cdecl cache_file_geometry_cache_for_models_open(Cache::s_cache_tag_header *tag_header);
		//
		// void __cdecl cache_file_geometry_cache_for_models_close();

		// void __cdecl cache_file_geometry_cache_for_bsp_open(TagGroups::structure_bsp_header *bsp_header);
		//
		// void __cdecl cache_file_geometry_cache_for_bsp_close(TagGroups::structure_bsp_header *bsp_header);

		// bool __cdecl cache_file_header_verify(Cache::s_cache_header *header, const char *  scenario_name, bool critical);
		//
		// datum_index cache_file_tags_load(const char *  scenario_name);
		//
		// void cache_file_tags_unload();
		//
		// void cache_file_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference *reference);
		//
		// bool cache_file_structure_bsp_load(TagGroups::scenario_structure_bsp_reference *reference);
	};
};