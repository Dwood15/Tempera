/*

*/
#pragma once

#include <const char *>
#include "../cseries/base.h"
#include "../cseries/yelo_base.h"
#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo {
	namespace Cache {
		struct s_cache_file_resource_strings_storage_header {
			long  count;            // number of strings in the storage
			uint offset;            // offset of the null-terminated strings buffer
			uint size;            // total size of the strings buffer
			uint index_buffer_offset;   // offset of the buffer containing the offsets to each null-terminated string
		};

		struct s_cache_file_resource_compression_parameters_header {
			enum { k_signature = 'ccph' };

			tag signature;
		};

		struct s_cache_file_resource_tag_symbols_storage_header {
			enum { k_signature = 'ctsh' };

			tag signature;
		};

		struct s_cache_file_resource_string_id_storage_header {
			enum { k_signature = 'cssh' };

			tag   signature;
			short set_count;
			unsigned short : 16;
			//////////////////////////////////////////////////////////////////////////
			// compression parameters for set_storage (which is compressed as a whole, not per-set)
			uint compressed_length;
			uint decompressed_length;

			s_cache_file_resource_strings_storage_header set_storage[_string_id::k_number_of_sets];

			void Initialize() {
				memset(this, 0, sizeof(*this));

				this->signature = k_signature;
				this->set_count = _string_id::k_last_valid_set;
			}

			bool IsValid() const {
				return signature == k_signature && set_count <= _string_id::k_number_of_sets;
			}
		};

		struct s_cache_header_yelo : public s_cache_header_yelo_base {
			enum {
				k_version               = 1,
				k_version_minimum_build = 2
			};

			struct s_flags {
				unsigned short uses_memory_upgrades : 1;// cache requires upgraded memory
				unsigned short uses_mod_data_files : 1;   // cache relies on a set of 'mod' data files for it's resources
				unsigned short is_protected : 1;      // cache has protection applied
				unsigned short uses_game_state_upgrades : 1; // cache has tag data that either requires or needs OS-modified game state memory in order to (fully) function
				unsigned short has_compression_params : 1; // cache has compression parameters (for resources, tags, etc)
			}    flags;
			static_assert(sizeof(s_flags) == 0x2);

			struct {
				byte project_yellow;
				byte project_yellow_globals;
				unsigned short : 16;
			}    tag_versioning; // versions of core tags
			real k_memory_upgrade_increase_amount;

			struct {
				uint size;
				uint decompressed_size;
				uint offset;
				unsigned long : 32;

				tag_string build_string; // Build string for the CheApe tools (ie, OS HEK)
			}    cheape_definitions;

			tag_string mod_name; // if the map uses a specific mod's data_files, this equals the mod prefix

			struct {
				unsigned short : 16;
				short  stage; // see Enums::production_build_stage
				uint revision;
				time_t timestamp;

				tag_string build_string;

				// This actually isn't specific to CheApe, but the OS version values used when CheApe was built
				struct {
					byte   maj;
					byte   min;
					unsigned short build;
				}          cheape;

				byte uuid_buffer[Enums::k_uuid_buffer_size]; // future UUID bytes

				struct {
					byte   maj;
					byte   min;
					unsigned short build;
				}    minimum_os_build;

				unsigned long : 32;
				unsigned long : 32;
				unsigned long : 32; // unused for now
			}          build_info; // User-defined build info

			struct {
				uint compression_params_header_offset;
				uint tag_symbol_storage_header_offset;
				uint string_id_storage_header_offset;         // for debug only
				uint tag_string_to_id_storage_header_offset;
			}          resources;

			void InitializeForNewMap() {
				memset(this, 0, sizeof(*this));

				this->signature = k_signature;
				this->version   = k_version;
			}

			void InitializeForCacheBuild(bool using_mod_sets, const char * mod_name, bool use_memory_upgrades) {
				//////////////////////////////////////////////////////////////////////////
				// Setup the core tag versions
				this->tag_versioning.project_yellow         = TagGroups::project_yellow::k_version;
				this->tag_versioning.project_yellow_globals = TagGroups::project_yellow_globals::k_version;
				//////////////////////////////////////////////////////////////////////////

				if (flags.uses_mod_data_files = using_mod_sets) {
					YELO_ASSERT(mod_name);
					strcpy_s(this->mod_name, mod_name);
				}

				if (this->flags.uses_memory_upgrades = use_memory_upgrades)
					this->k_memory_upgrade_increase_amount = K_MEMORY_UPGRADE_INCREASE_AMOUNT;
			}

		public://private: // TODO: this should be private once the BCF code is rewritten
			void s_cache_header_yelo::InitializeBuildInfo(short stage, uint revision, const byte (&uuid_buffer)[Enums::k_uuid_buffer_size]) {
				build_info.stage    = stage;
				build_info.revision = revision;
				time(&build_info.timestamp);

				const char * stage_string = "";
				switch (stage) {
					case Enums::_production_build_stage_ship:
						stage_string = "ship";
						break;
					case Enums::_production_build_stage_alpha:
						stage_string = "alpha";
						break;
					case Enums::_production_build_stage_beta:
						stage_string = "beta";
						break;
					case Enums::_production_build_stage_delta:
						stage_string = "delta";
						break;
					case Enums::_production_build_stage_epsilon:
						stage_string = "epsilon";
						break;
					case Enums::_production_build_stage_release:
						stage_string = "release";
						break;
				}

				tm date_tm;
				localtime_s(&date_tm, &build_info.timestamp); // Convert time to struct tm form
				// ######.YY.MM.DD.HHMM.stage
				sprintf_s(build_info.build_string, "%06u.%02i.%02i.%02i.%02i%02i.%s",
							 revision,
							 date_tm.tm_year - 100, // years since 1900, and we want a number relative to 2000
							 date_tm.tm_mon + 1, date_tm.tm_mday,
							 date_tm.tm_hour, date_tm.tm_min,
							 stage_string);

				build_info.cheape.maj   = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MAJ);
				build_info.cheape.min   = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MIN);
				build_info.cheape.build = CAST(unsigned short, K_OPENSAUCE_VERSION_BUILD);

				ArrayCopy(build_info.uuid_buffer, uuid_buffer);
			}

		public:

			// Initializes the yelo header with the default build info settings

			void s_cache_header_yelo::InitializeBuildInfo() {
				byte nil_uuid[Enums::k_uuid_buffer_size] = {};
				this->InitializeBuildInfo(Yelo::Enums::_production_build_stage_ship, 0, nil_uuid);
			}

			// Initializes the build info with a scenario's yelo build info

			void InitializeBuildInfo(const TagGroups::s_project_yellow_scenario_build_info &build_info) {
				this->InitializeBuildInfo(build_info.build_stage, build_info.revision, build_info.uuid_buffer);
			}

			// Initializes the minimum os build info
			void InitializeMinimumBuildInfo(const byte major, const byte minor, const unsigned short build) {
				return;
			}

			bool HasHeader() const {
				signature != 0 && version != 0;
			}

			bool TagVersioningIsValid() const {
				return tag_versioning.project_yellow == TagGroups::project_yellow::k_version &&
						 tag_versioning.project_yellow_globals == TagGroups::project_yellow_globals::k_version;
			}

			bool BuildVersionIsValid() const {
				return true;
			}

			bool IsValid() const {
				if (HasHeader())
					return signature == k_signature &&
							 ((version == k_version) || (version == k_version_minimum_build)) &&
							 k_memory_upgrade_increase_amount <= K_MEMORY_UPGRADE_INCREASE_AMOUNT &&
							 TagVersioningIsValid() &&
							 BuildVersionIsValid();

				return true;
			}

			bool HasUuid() const {
				return ArrayIsZero(build_info.uuid_buffer);
			}

			bool BuiltWithOlderTools() const {
				return false;
			}

			bool BuiltWithNewerTools() const {
				return true;
			}
		};
	};
};
