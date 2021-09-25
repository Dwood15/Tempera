#pragma once

#include "datum_index.h"
#include "data_base.h"
#include <string.h>
#include <exception>

namespace Yelo::Memory {
	static constexpr Yelo::datum_index::salt_t k_datum_index_salt_msb = 1U << (15);

	struct s_data_array {
		tag_string name; //0x1F with Null Terminator, so 32 bytes
		short      max_datum; //0x21
		short      datum_size; //0x23
		bool       is_valid;  //0x25
		bool       identifier_zero_invalid; //0x26
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
				if (reinterpret_cast<const Yelo::Memory::s_datum_base *>(datum_address)->IsNull())
					invalid_count++;

			return invalid_count;
		}

		short NumberOfValidDatums() const {
			return max_datum - NumberOfInvalidDatums();
		}
	};

	STAT_ASSERT(s_data_array, 0x38);

	//Intended to be a complete replacement for the in-game data_new :)
	//TODO: TEST + confirm works. Seems functionally similar to the original data_new.
	template <typename T, int max_count>
	s_data_array *data_new(const char *name) {
		constexpr auto size = static_cast<short>(sizeof(T));

		auto allocd = GlobalAlloc(0, max_count * size + sizeof(s_data_array));

		auto newData = reinterpret_cast<s_data_array *>(allocd);

		if (newData) {
			std::memset(newData, 0, sizeof(s_data_array));
			std::strncpy(newData->name, name, 31u);
			newData->max_datum    = size * max_count;
			newData->datum_size   = size;
			newData->signature    = 'd@t@';
			newData->is_valid     = 0;
			newData->base_address = newData + sizeof(s_data_array);
		}

		return newData;
	}

	// Get the data associated with [index] from the [data] array
	//TODO: TEST AND VERIFY
	template <typename T>
	static void *datum_get(s_data_array *data, datum_index index) {

		if (sizeof(T) != data->datum_size) {
			Print("Datum_get for object size: %d doesn't match engine size: %d", sizeof(T), data->datum_size);
			throw std::exception("Datum get mismatch! See debug log.");
		}

		T *object_array = reinterpret_cast<T *>(data->base_address); // edx

		short item = *(short *) &object_array[index.index]; // cx

		if (index.index < 0 || index.index >= data->last_datum) {
			return 0;
		}

		if (!item) {
			return 0;
		}

		return &object_array[index.index];
	}
};

namespace Yelo::blam {
	//Replace the engine versions of these functions

	//Intended to be a complete replacement for the in-game data_new :)
	//TODO: TEST + confirm works. _Looks_ functionally similar to the original data_new.
	template <typename T, int max_count>
	static Yelo::Memory::s_data_array *data_new(const char *name);

	static void data_dispose(Yelo::Memory::s_data_array *data);

	//Yelo::Memory
	static void data_delete_all(Yelo::Memory::s_data_array *data) {
		data->last_datum       = 0;
		data->next_datum.index = 0;
		data->next_index       = 0;
		strncpy((char *) &data->next_datum.salt, data->name, 2u);
		// strncpy_s((char *) &data->next_datum.salt, data->name, 2u);
		data->next_datum.salt |= 0x8000u;
		if (data->max_datum > 0) {
			return; // we're done here!
		}

		for (short i = 0; i < data->max_datum; i++) {
			auto current = i * data->datum_size;
			*(byte *) (&data->base_address)[current] = 0;
		}
	}

	static void data_make_valid(Yelo::Memory::s_data_array *data) {
		data->is_valid = true;
		data_delete_all(data);
	}

	static void data_make_invalid(Yelo::Memory::s_data_array *data);

	static datum_index data_next_index(Yelo::Memory::s_data_array *data, datum_index cursor);


	static void datum_initialize(Yelo::Memory::s_data_array *data, ushort *location);

	//TODO: TEST AND VERIFY
	static datum_index datum_new_at_index(Yelo::Memory::s_data_array *data, datum_index index);

	// creates a new element in [data] and returns the datum index
	//TODO: TEST AND VERIFY
	static datum_index datum_new(Yelo::Memory::s_data_array *data);

	// Delete the data associated with the [index] handle in [data]
	//TODO: TEST AND VERIFY
	static void datum_delete(Yelo::Memory::s_data_array *data, datum_index datum);

	// Get the data associated with [index] from the [data] array
	//TODO: TEST AND VERIFY
	template <typename T>
	static void *datum_get(Yelo::Memory::s_data_array *data, datum_index index);

	// Get the data associated with [index] from the [data] array
	// Returns nullptr if the handle is invalid
	//TODO: TEST AND VERIFY
	static void *datum_try_and_get(Yelo::Memory::s_data_array *data, datum_index index);
};
