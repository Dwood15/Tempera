#pragma once

namespace Yelo::Cache {
	constexpr const char *  K_DATA_FILE_EXTENSION = ".map";
	constexpr const char *K_MAP_FILE_EXTENSION  = ".map";
	constexpr const char *K_MAP_FILES_DIRECTORY = R"(maps\)";
};

#include "generic_enums.h"
namespace Yelo::Enums {
	enum cache_file_request_source : unsigned char {
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
