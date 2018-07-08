#pragma once


// Terminology:
// maps_path: The system directory path to a "maps\" folder.
//				Generally one should be under the CWD, or the user profile. Dedicated server's default behavior is to
//				query the registry for the client's "EXE Path" before it checks the CWD.
// map_path: The system file path to a given cache file
// map_name: The name of a cache file without path information or extensions

#include <synchapi.h>
#include "../cseries/base.h"
#include "../memory/datum_index.h"
#include "../tags/group/tag_groups.h"
#include "cache_files_structures.h"

namespace Yelo {
	namespace Enums {
		enum cache_file_request_source : byte_enum {
			_cache_file_request_source_open_map,
			_cache_file_request_source_bitmaps,
			_cache_file_request_source_sounds,
			_cache_file_request_source_loc,
		};

		enum cache_file_type : short {
			_cache_file_type_campaign    = _scenario_type_campaign,
			_cache_file_type_multiplayer = _scenario_type_multiplayer,
			_cache_file_type_main_menu   = _scenario_type_main_menu,
		};
	};

	namespace Cache {
	struct s_cache_file_request_params {
		typedef void (__cdecl *proc_finished)(void *params);
			bool          *finished_flag;// NOT optional
			void          *finished_proc_params;   // optional
			proc_finished finished_proc;// optional
		};

	static_assert(sizeof(s_cache_file_request_params) == 0xC);

		const char *  K_MAP_FILE_EXTENSION; // ".maps"
		const char *  K_MAP_FILES_DIRECTORY;// "maps\"


		s_cache_file_globals *CacheFileGlobals();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Directory to the maps\ folder for the defined PLATFORM_TYPE. </summary>
		///
		/// <remarks>For editors, this is the maps path as defined by the editor profile settings. For the runtime, this is
		/// 	just "maps\".
		/// </remarks>
		///
		/// <returns>	. </returns>
		const char *  MapsDirectory();

		// if true, data which is in a data file won't be loaded during the cache_file's tags load process
		bool DontLoadExternalData();
	};



	namespace blam {
		bool __cdecl cache_file_read_request(/*datum_index tag_index,*/ // unused, and optimized out, at runtime
			uint offset, uint size, void *buffer, const Cache::s_cache_file_request_params &params,
			bool block = true, Enums::cache_file_request_source source = Enums::_cache_file_request_source_open_map);

		// made up names; H1 didn't have a "geometry_cache" like it does for textures and sounds
		// void __cdecl cache_file_geometry_cache_for_models_open(Cache::s_cache_tag_header *tag_header);
		//
		// void __cdecl cache_file_geometry_cache_for_models_close();

		void __cdecl cache_file_geometry_cache_for_bsp_open(TagGroups::structure_bsp_header *bsp_header);

		void __cdecl cache_file_geometry_cache_for_bsp_close(TagGroups::structure_bsp_header *bsp_header);

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
		struct structure_bsp_header;
		struct scenario_structure_bsp_reference;

		Cache::s_cache_tag_header *Index();

		Cache::s_cache_tag_instance const *Instances(); // Don't directly use this for getting tags
	};

	namespace Cache {
		const char *  K_MAP_FILE_EXTENSION  = ".map";
		const char *  K_MAP_FILES_DIRECTORY = R"(maps\)";

		// this bool is actually defined in the engine, but the only reference is in the cache_file's tags load process
		static bool g_dont_load_external_data;

		constexpr const char *  MapsDirectory() { return K_MAP_FILES_DIRECTORY; }

		inline bool DontLoadExternalData() { return g_dont_load_external_data; }

		// bool s_cache_header::ValidSignatures() const {
		// 	return header_signature == k_header_signature && footer_signature == k_footer_signature;
		// }
		//
		// bool s_cache_header::ValidFileSize(long max) const {
		// 	return file_length >= 0 && file_length <= max;
		// }
		//
		// bool s_cache_header::ValidName() const {
		// 	return strnlen_s(name, Enums::k_tag_string_length + 1) <= Enums::k_tag_string_length;
		// }
		//
		// bool s_cache_tag_instance::MatchesGroup(tag group_tag) const {
		// 	return this->group_tag == group_tag || parent_groups[0] == group_tag || parent_groups[1] == group_tag;
		// }
		//
		// bool s_cache_header::ValidForStock() const {
		// 	if (!ValidSignatures()) { return false; }
		//
		// 	if (!ValidFileSize(Enums::k_max_cache_size)) { return false; }
		//
		// 	if (!ValidName()) { return false; }
		//
		// 	if (version != k_version) { return false; }
		//
		// 	return true;
		// }
		//
		// bool s_cache_header::ValidForYelo() const {
		// 	if (!ValidSignatures())
		// 		return false;
		//
		// 	if (!ValidFileSize(Enums::k_max_cache_size_upgrade))
		// 		return false;
		//
		// 	if (!ValidName())
		// 		return false;
		//
		// 	if (version != k_version)
		// 		return false;
		//
		// 	return true;
		// }
		//
		// short s_cache_file_globals::FindMapFileIndexByName(const char *  scenario_name) {
		// 	for (short map_file_index = 0; map_file_index < std::size(map_files); map_file_index++) {
		// 		if (!_stricmp(scenario_name, map_files[map_file_index].header.name))
		// 			return map_file_index;
		// 	}
		//
		// 	return NONE;
		// }

		// bool s_cache_file_globals::OpenMapFileOpen(const char *  scenario_name, s_cache_header *header) {
		// 	//assert(scenario_name);
		// 	//assert(header);
		// 	//assert(open_map_file_index == NONE);
		//
		// 	short map_file_index = FindMapFileIndexByName(scenario_name);
		// 	//assert(map_file_index != NONE);
		//
		// 	memset(Requests(), 0, sizeof(*Requests()) * Enums::k_maximum_simultaneous_cache_requests);
		// 	open_map_file_index = map_file_index;
		// 	memcpy(header, &map_files[map_file_index].header, sizeof(*header));
		//
		// 	return true;
		// }

		// void s_cache_file_globals::OpenMapFileClose() {
		// 	auto *open_map_file = OpenMapFile();
		// 	if (open_map_file != nullptr) {
		// 		RequestsWaitAll();
		// 		open_map_file->Close();
		// 		open_map_file_index = NONE;
		// 	}
		// }
		//
		// void s_cache_file_globals::RequestsWaitAll() {
		// 	s_cache_file_request *request = Requests();
		// 	for (rsize_t         x        = Enums::k_maximum_simultaneous_cache_requests; x > 0; x--, request++) {
		// 		const bool &request_is_reading = request->reading;
		//
		// 		while (request_is_reading)
		// 			Sleep(0);
		// 	}
		// }
	};

	namespace TagGroups {
		// defined by the implementing shell
		Cache::s_cache_tag_instance **GlobalTagInstancesReference();

#define blam_global_tag_instances   *(TagGroups::GlobalTagInstancesReference())

		bool TagIsInstanceOf(datum_index tag_index, tag group_tag) {
			if (!tag_index.IsNull() && tag_index.index < Index()->count) {
				Cache::s_cache_tag_instance const &instance = Instances()[tag_index.index];

				return instance.MatchesGroup(group_tag);
			}

			return false;
		}

		// void *TagTryAndGetImpl(datum_index tag_index, tag group_tag) {
		// 	if (!tag_index.IsNull() && tag_index.index < Index()->count) {
		// 		Cache::s_cache_tag_instance const &instance = Instances()[tag_index.index];
		//
		// 		if (instance.MatchesGroup(group_tag))
		// 			return instance.base_address;
		// 	}
		//
		// 	return nullptr;
		// }
		//
		// void *TagGetUnsafeImpl(datum_index tag_index) {
		// 	Cache::s_cache_tag_instance const &instance = Instances()[tag_index.index];
		//
		// 	return instance.base_address;
		// }
	};

	namespace blam {
		using namespace Cache;

		static s_cache_tag_instance *cache_file_tag_get_instance(datum_index tag_index) {
			auto &cache_file_globals = *CacheFileGlobals();

			auto *global_tag_instances = TagGroups::Instances();

			auto &cache_tag_header = *Yelo::TagGroups::Index();
			//YELO_ASSERT_DISPLAY(tag_index.index > NONE && tag_index.index < cache_tag_header.count, "i don't think %08x is a tag index", tag_index);

			auto &tag_instance = global_tag_instances[tag_index.index];
			if (tag_index.salt != NONE) {
				//YELO_ASSERT_DISPLAY(tag_instance.handle == tag_index, "i don't think %08x is a tag index", tag_index);
			}

			return (const_cast<s_cache_tag_instance *>(&tag_instance));
		}

		tag __cdecl tag_get_group_tag(datum_index tag_index) {
			return cache_file_tag_get_instance(tag_index)->group_tag;
		}

		// void __cdecl cache_file_geometry_cache_for_models_open(Cache::s_cache_tag_header *tag_header) {
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// }
		//
		// void __cdecl cache_file_geometry_cache_for_models_close() {
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// }
		//
		// void __cdecl cache_file_geometry_cache_for_bsp_open(TagGroups::structure_bsp_header *bsp_header) {
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// }
		//
		// void __cdecl cache_file_geometry_cache_for_bsp_close(TagGroups::structure_bsp_header *bsp_header) {
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// }
		//
		// bool __cdecl cache_file_header_verify(Cache::s_cache_header *header, const char *  scenario_name, bool critical) {
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		//
		// 	return false;
		// }

		using namespace TagGroups;

		// static void cache_file_data_load_sound_resources(s_cache_tag_instance *tag_instance, s_cache_file_data_load_state &state) {
		// 	byte *data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_sounds);
		//
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// 	auto *sound_definition = tag_instance->Definition<TagGroups::sound_definition>();
		// }
		//
		// static void cache_file_data_load_bitmap_resources(s_cache_tag_instance *tag_instance, s_cache_file_data_load_state &state) {
		// 	byte *data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_bitmaps);
		//
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		// 	auto *bitmap_group = tag_instance->Definition<TagGroups::s_bitmap_group>();
		// }
		//
		// static void cache_file_data_load_multilingual_strings_resources(s_cache_tag_instance *tag_instance, byte *data_base_address) {
		// }
		//
		// static void cache_file_data_load_font_resources(s_cache_tag_instance *tag_instance, byte *data_base_address) {
		// }
		//
		// static void cache_file_data_load_hud_message_text_resources(s_cache_tag_instance *tag_instance, byte *data_base_address) {
		// 	auto *hud_state_messages = tag_instance->Definition<TagGroups::hud_state_messages>();
		// }

		// static void cache_file_data_load_ui_resources(s_cache_tag_instance *tag_instance, s_cache_file_data_load_state &state) {
		// 	byte *data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_locale);
		//
		// 	//YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		//
		// 	switch (tag_instance->group_tag) {
		// 		case 'ustr':
		// 			cache_file_data_load_multilingual_strings_resources(tag_instance, data_base_address);
		// 			break;
		// 		case 'font':
		// 			cache_file_data_load_font_resources(tag_instance, data_base_address);
		// 			break;
		// 		case TagGroups::hud_state_messages::k_group_tag:
		// 			cache_file_data_load_hud_message_text_resources(tag_instance, data_base_address);
		// 			break;
		//
		// 			//YELO_ASSERT_CASE_UNREACHABLE();
		// 	}
		// }

		// static void cache_file_data_load(Cache::s_cache_tag_header *tag_header, s_cache_file_data_load_state &state) {
		// 	for (datum_index tag_index : TagGroups::c_tag_iterator::all()) {
		// 		s_cache_tag_instance *tag_instance = cache_file_tag_get_instance(tag_index);
		// 		if (!tag_instance->bool_in_data_file)
		// 			continue;
		//
		// 		switch (tag_instance->group_tag) {
		// 			case TagGroups::sound_definition::k_group_tag:
		// 				cache_file_data_load_sound_resources(tag_instance, state);
		// 				break;
		//
		// 			case TagGroups::s_bitmap_group::k_group_tag:
		// 				cache_file_data_load_bitmap_resources(tag_instance, state);
		// 				break;
		//
		// 			case 'ustr':
		// 			case 'font':
		// 			case TagGroups::hud_state_messages::k_group_tag:
		// 				cache_file_data_load_ui_resources(tag_instance, state);
		// 				break;
		// 		}
		// 	}
		// }

		// datum_index cache_file_tags_load(const char *  scenario_name) {
		// 	auto &cache_file_globals  = *CacheFileGlobals();
		// 	// //YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		//
		// 	scenario_name = tag_name_strip_path(scenario_name);
		// 	auto *cache_header = &cache_file_globals.cache_header;
		// 	if (cache_file_globals.OpenMapFileOpen(scenario_name, cache_header) ||
		// 		 !cache_file_header_verify(cache_header, scenario_name, true)) {
		// 		return datum_index::null();
		// 	}
		//
		// 	auto *tag_header = reinterpret_cast<Cache::s_cache_tag_header *>(physical_memory_map_get_tag_cache_address());
		//
		// 	bool                        read_finished = false;
		// 	s_cache_file_request_params read_params   = {&read_finished};
		// 	cache_file_read_request(cache_header->offset_to_index, cache_header->tag_memory_size, tag_header,
		// 									read_params);
		// 	while (!read_finished)
		// 		Sleep(0);
		//
		// 	// //YELO_ASSERT_DISPLAY(tag_header->signature == Cache::s_cache_tag_header::k_signature,
		// 	// 						  "signature is '%s', should be '%s'",
		// 	// 						  TagGroups::group_tag_to_string{tag_header->signature}.ToString(),
		// 	// 						  TagGroups::group_tag_to_string{Cache::s_cache_tag_header::k_signature}.ToString());
		// 	cache_file_globals.tag_index = tag_header;
		//
		// 	blam_global_tag_instances = reinterpret_cast<s_cache_tag_instance *>(tag_header->tags_address);
		// 	cache_file_globals.tags_loaded = true;
		//
		// 	if (!Cache::DontLoadExternalData()) {
		// 		auto *scenario       = tag_get<TagGroups::scenario>(tag_header->scenario_index);
		// 		auto *hs_syntax_data = reinterpret_cast<Memory::s_data_array *>(scenario->hs_syntax_data.address);
		// 		// the data array's base address pointer isn't updated when a cache is built, so it's updated here
		// 		if (hs_syntax_data != nullptr) // technically, this should never be NULL (engine doesn't perform check)
		// 		{
		// 			hs_syntax_data->base_address = hs_syntax_data + 1;
		// 		}
		//
		// 		s_cache_file_data_load_state load_state(cache_header, tag_header);
		// 		cache_file_data_load(tag_header, load_state);
		// 	}
		//
		// 	cache_file_geometry_cache_for_models_open(tag_header);
		//
		// 	return tag_header->scenario_index;
		// }
		//
		// void cache_file_tags_unload() {
		// 	auto &cache_file_globals  = *CacheFileGlobals();
		//
		// 	cache_file_globals.OpenMapFileClose();
		// 	cache_file_geometry_cache_for_bsp_close(cache_file_globals.structure_bsp_header);
		// 	cache_file_geometry_cache_for_models_close();
		// 	cache_file_globals.tags_loaded = false;
		//
		// 	blam_global_tag_instances = nullptr;
		// }

		void cache_file_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference *reference) {
			auto &cache_file_globals = *CacheFileGlobals();

			cache_file_geometry_cache_for_bsp_close(cache_file_globals.structure_bsp_header);

			auto *tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			//assert(tag_instance->base_address);
			//assert(tag_instance->group_tag == structure_bsp::k_group_tag);

			tag_instance->base_address              = nullptr;
			cache_file_globals.structure_bsp_header = nullptr;
		}

		bool cache_file_structure_bsp_load(TagGroups::scenario_structure_bsp_reference *reference) {
			auto &cache_file_globals = *CacheFileGlobals();

			bool                               finished_flag = false;
			Cache::s_cache_file_request_params read_params;
			read_params.finished_flag = &finished_flag;

			cache_file_read_request(reference->cache_offset, reference->bsp_data_size, reference->header, read_params);
			while (!finished_flag);

			cache_file_globals.structure_bsp_header = reference->header;
			// actually asserts with CACHE_FILE_STRUCTURE_BSP_HEADER_SIGNATURE
			//assert(cache_file_globals.structure_bsp_header->signature == structure_bsp::k_group_tag);

			cache_file_geometry_cache_for_bsp_open(cache_file_globals.structure_bsp_header);

			auto *tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			//assert(!tag_instance->base_address);
			//assert(tag_instance->group_tag == structure_bsp::k_group_tag);

			tag_instance->base_address = cache_file_globals.structure_bsp_header->bsp;

			return false;
		}
	};
};
