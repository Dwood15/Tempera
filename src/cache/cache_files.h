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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
			switch (type) {
				case Enums::_data_file_type_bitmaps:
					return "bitmaps";
				case Enums::_data_file_type_sounds:
					return "sounds";
				case Enums::_data_file_type_locale:
					return "loc";
			}
#pragma clang diagnostic pop
			throw "What type of DataFile are you looking for???";
		}

		static const char *  DataFileTypeToString(Enums::data_file_reference_type type) {
			return DataFileTypeToString((Enums::data_file_type)(type - Enums::_data_file_reference_bitmaps));
		}



		// if true, data which is in a data file won't be loaded during the cache_file's tags load process
		bool DontLoadExternalData();
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

namespace Yelo {

	namespace TagGroups {
		Cache::s_cache_tag_header *Index();

		Cache::s_cache_tag_instance const *Instances(); // Don't directly use this for getting tags
	};

	namespace Cache {

		// this bool is actually defined in the engine, but the only reference is in the cache_file's tags load process
		static bool g_dont_load_external_data;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Directory to the maps\ folder for the defined PLATFORM_TYPE. </summary>
		///
		/// <remarks>For editors, this is the maps path as defined by the editor profile settings. For the runtime, this is
		/// 	just "maps\".
		/// </remarks>
		///
		/// <returns>	. </returns>
		constexpr const char *MapsDirectory() { return K_MAP_FILES_DIRECTORY; }

		inline bool DontLoadExternalData() { return g_dont_load_external_data; }
	};

	namespace TagGroups {
		// defined by the implementing shell
		static Cache::s_cache_tag_instance **GlobalTagInstancesReference();

#define blam_global_tag_instances   *(TagGroups::GlobalTagInstancesReference())

		static bool TagIsInstanceOf(datum_index tag_index, tag group_tag) {
			if (!tag_index.IsNull() && tag_index.index < Index()->count) {
				Cache::s_cache_tag_instance const &instance = Instances()[tag_index.index];

				return instance.MatchesGroup(group_tag);
			}

			return false;
		}
	};

	namespace blam {
		using namespace Cache;

		static s_cache_tag_instance *cache_file_tag_get_instance(datum_index tag_index) {

			auto *global_tag_instances = TagGroups::Instances();

			// auto &cache_tag_header = *Yelo::TagGroups::Index();
			//YELO_ASSERT_DISPLAY(tag_index.index > NONE && tag_index.index < cache_tag_header.count, "i don't think %08x is a tag index", tag_index);

			auto &tag_instance = global_tag_instances[tag_index.index];
			if (tag_index.salt != NONE) {
				//YELO_ASSERT_DISPLAY(tag_instance.handle == tag_index, "i don't think %08x is a tag index", tag_index);
			}

			return (const_cast<s_cache_tag_instance *>(&tag_instance));
		}

		static tag __cdecl tag_get_group_tag(datum_index tag_index) {
			return cache_file_tag_get_instance(tag_index)->group_tag;
		}
	};
};
