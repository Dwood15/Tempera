#pragma once

#include "cache_files.h"
#include "../cseries/base.h"
#include "data_file_structures.h"
#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo {
	namespace Enums {
		enum data_file_type {
			_data_file_type_bitmaps,
			_data_file_type_sounds,
			_data_file_type_locale,

			k_number_of_data_file_types,
		};

		enum data_file_reference_type : short {
			_data_file_reference_none,
			_data_file_reference_bitmaps,
			_data_file_reference_sounds,
			_data_file_reference_locale,
		};
	};

	namespace Cache {
		constexpr const char *  K_DATA_FILE_EXTENSION = ".map";

		static const char *  DataFileTypeToString(Enums::data_file_type type) {
			switch (type) {
				case Enums::_data_file_type_bitmaps:
					return "bitmaps";
				case Enums::_data_file_type_sounds:
					return "sounds";
				case Enums::_data_file_type_locale:
					return "loc";
			}
			// throw "What type of DataFile are you looking for???";
			return nullptr;
		}

		static const char *  DataFileTypeToString(Enums::data_file_reference_type type) {
			return DataFileTypeToString(reinterpret_cast<Enums::data_file_type>(type - Enums::_data_file_reference_bitmaps));
		}

		s_data_file &DataFileGet(Enums::data_file_reference_type data_file) {
			auto &globals = CacheFileGlobals()->data_files;

			return globals.Get(data_file);
		}

		// s_data_file &s_data_file_globals::Get(Enums::data_file_reference_type data_file) {
		// 	switch (data_file) {
		// 		case Enums::_data_file_reference_bitmaps:
		// 			return this->bitmaps;
		// 		case Enums::_data_file_reference_sounds:
		// 			return this->sounds;
		// 		case Enums::_data_file_reference_locale:
		// 			return this->locale;
		// 	}
		// 	return nullptr;
		// }

		// utility for blam::cache_file_data_load

		bool DataFileGetItemDataInfo(Enums::data_file_reference_type data_file, long item_index, long &out_data_offset, long &out_data_size) {
			auto &df = DataFileGet(data_file);

			return df.GetItemDataInfo(item_index, out_data_offset, out_data_size);
		}

		// utility for blam::cache_file_data_load
		bool DataFileReadItemData(Enums::data_file_reference_type data_file, uint position, void *buffer, size_t buffer_size) {
			auto &df = DataFileGet(data_file);

			return df.ReadItemData(position, buffer, buffer_size);
		}

		bool DataFilesOpen(const char *  bitmaps_path, const char *  sounds_path, const char *  locale_path, bool store_resources = false) {
			using namespace Enums;

			auto bitm_succ = DataFileGet(_data_file_reference_bitmaps).Open(_data_file_reference_bitmaps, store_resources, bitmaps_path);
			auto snd_succ  = DataFileGet(_data_file_reference_sounds).Open(_data_file_reference_sounds, store_resources, sounds_path);
			auto loc_succ  = DataFileGet(_data_file_reference_locale).Open(_data_file_reference_locale, store_resources, locale_path);

			return (bitm_succ && snd_succ && loc_succ);
		}
	};

	namespace blam {
		// bool data_files_close() {
		// 	auto bitm_succ = data_file_close(Enums::_data_file_reference_bitmaps);
		// 	auto snd_succ  = data_file_close(Enums::_data_file_reference_sounds);
		// 	auto loc_succ  = data_file_close(Enums::_data_file_reference_locale);
		//
		// 	return (bitm_succ && snd_succ && loc_succ);
		//
		// }
		//
		// bool data_file_close(Enums::data_file_reference_type data_file) {
		// 	auto &file = Cache::DataFileGet(data_file);
		// 	return file.Close();
		// }
	};
};
