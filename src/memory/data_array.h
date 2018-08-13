#pragma once
#include "datum_index.h"
#include "data_base.h"

namespace Yelo::Memory {
	static constexpr Yelo::datum_index::salt_t k_datum_index_salt_msb = 1U << (15);

	struct s_data_array {
		tag_string name;
		short      max_datum;
		short      datum_size;
		bool       is_valid;
		bool       identifier_zero_invalid;
		unsigned short : 16;
		tag         signature;
		short       next_index;      // Next index to use when creating a new datum
		short       last_datum;      // Last used datum index
		datum_index next_datum;   // Next datum value to use
		/// <summary>	Base address of the array's datum. </summary>
		void        *base_address;

		// datum_index::salt_t GetInitialSalt() const {
		// 	datum_index::salt_t result = 0;
		// 	// NOTE: engine uses strncpy
		// ::std::memcpy(&result, this->name, sizeof(result));
		//
		// 	result |= k_datum_index_salt_msb;
		//
		// 	return result;
		// }

		datum_index::salt_t GetNextSalt(datum_index::salt_t cursor) const {
			return ++cursor != 0 ? cursor : k_datum_index_salt_msb;
		}

		short NumberOfInvalidDatums() const {

			auto  *datum_address = reinterpret_cast<const byte *>(base_address);
			short invalid_count  = 0;

			for (int x = 0, max_count = max_datum; x < max_count; x++, datum_address += datum_size)
				if (reinterpret_cast<const s_datum_base *>(datum_address)->IsNull())
					invalid_count++;

			return invalid_count;
		}

		short NumberOfValidDatums() const {
			return max_datum - NumberOfInvalidDatums();
		}
	};
	STAT_ASSERT(s_data_array, 0x38)


	static s_data_array *DataNewAndMakeValid(const char *name, long maximum_count, size_t datum_size) {
		//
		// Memory::s_data_array *data = blam::data_new(name, maximum_count, datum_size);
		//
		// if (data != nullptr) {
		// 	data->is_valid = true;
		// 	Yelo::blam::data_delete_all(data);
		// }
		//
		// return data;
		return (s_data_array *) -1;
	}
}
