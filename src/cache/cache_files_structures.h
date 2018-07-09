#pragma once

#include <windows.h>
#include "cache_base.h"
#include "cache_files_structures_yelo.h"
#include "cache_files.h"

namespace Yelo::Cache {
	struct s_cache_header : public s_cache_header_base {
		enum {
			k_version = 609,

			k_pad_size = 484 * sizeof(long),
		};

		tag header_signature;

		long version;
		long file_length;
		long compressed_file_length;

		long offset_to_index;
		long tag_memory_size;
		PAD64; // ?
		tag_string             name;
		tag_string             build_string;
		Yelo::Enums::cache_file_type cache_type;
		unsigned short : 16;
		uint crc;
		unsigned long : 32; // ?

		unsigned long : 32; // Align so the yelo header starts on a 16B boundary
		s_cache_header_yelo yelo;
		byte                padding[k_pad_size - (sizeof(uint) + sizeof(s_cache_header_yelo))];

		tag footer_signature;

		bool ValidSignatures() const;

		bool ValidFileSize(long max) const;

		bool ValidName() const;

		// Is this header valid according to the stock game's standards?
		bool ValidForStock() const;

		// Is this header valid according to OS standards?
		bool ValidForYelo() const;
	};

	static_assert(sizeof(s_cache_header) == 0x800);

	struct s_cache_tag_instance {
		tag         group_tag;         // 0x0
		tag         parent_groups[2];   // 0x4
		datum_index handle;      // 0xC
		const char *name;         // 0x10
		union {
			void *base_address;   // 0x14
			long index_in_data_file;
		};
		BOOL       bool_in_data_file;   // 0x18
		uint       _unused;         // 0x1C

		template <typename T>
		T *Definition() const { return reinterpret_cast<T *>(base_address); }

		long GetAbsoluteIndex() const { return handle.index; }

		// Is this an instance of a certain tag group?
		// If this instance a child of a certain tag group?
		bool MatchesGroup(tag group_tag) const;
	};

	static_assert(sizeof(s_cache_tag_instance) == 0x20);

	struct s_cache_tag_header {
		enum {
			k_signature = 'tags'
		};

		void *tags_address;         // 0x0
		datum_index scenario_index;   // 0x4
		uint        checksum;         // 0x8
		long        count;            // 0xC
		struct {
			long count;
			long offset;
		}           vertices, indices;         // 0x10, 0x18
		long        model_data_size;      // 0x20
		tag         signature;            // 0x24

		// nasty hack assumption
#pragma warning(push)
#pragma warning(disable : 4200)
		s_cache_tag_instance tags[];
#pragma warning(pop)
	}; static_assert(sizeof(s_cache_tag_header) == 0x28);
};
