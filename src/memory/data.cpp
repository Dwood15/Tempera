#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <enums/generic_enums.h>
#include <cstring>
#include <exception>
#include <addlog.h>
#include "data_base.h"
#include "data_array.h"
#include "data_iterator.h"
#include "../tags/group/tag_groups.h"
namespace Yelo::blam {
	using namespace Yelo::Memory;
	static void data_iterator_new(s_data_iterator &iterator, Yelo::Memory::s_data_array *data) {
		PrintLn("darat_iterator_new for: %s", TagGroups::group_tag_to_string(iterator.signature));
		if (!data->is_valid) {
			throw "invalid data array passed"; // __FUNCTION__);
		}
		iterator.data          = data;
		iterator.next_index    = 0;
		iterator.finished_flag = false;
		iterator.index         = datum_index::null();
		iterator.signature     = reinterpret_cast<uintptr_t>(data) ^ Enums::k_data_iterator_signature;
	}

	// [[deprecated]]
	// static s_data_array *data_new(const char *name, long maximum_count, size_t datum_size) {
	// 	static auto FUNCTION = CurrentRuntime->getFunctionBegin("data_new");
	//
	// 	if (!FUNCTION) {
	// 		return nullptr;
	// 	}
	//
	// 	__asm mov      ebx, datum_size
	// 	return calls::DoCall<Convention::m_cdecl, s_data_array *, long, const char *>(*FUNCTION, maximum_count, name);
	// }


	static void data_dispose(s_data_array *data) {
		if (data != nullptr) {
			GlobalFree(data);
		}
	}

	static void data_make_invalid(s_data_array *data) {
		data->is_valid = false;
	}

	static datum_index data_next_index(s_data_array *data, datum_index cursor) {
		auto result = datum_index::null();

		if (data == nullptr || cursor.IsNull()) {
			return result;
		}

		if (++cursor.index >= 0) {
			ushort last = data->last_datum;
			if (cursor.index < last) {
				auto sz = data->datum_size;
				//TODO: Simplify
				ushort *next = (ushort *) ((uint) data->base_address + (sz * cursor.index));
				while (!*next) {
					++cursor.index;
					next += sz;
					if (cursor.index >= last) {
						return result;
					}
				}
				result.salt  = *next;
				result.index = cursor.index;
			}
		}

		return result;
	}

	s_data_iterator  *data_iterator_next(s_data_iterator *iterator) {
		if (iterator->signature != (reinterpret_cast<uintptr_t>(iterator->data) ^ Enums::k_data_iterator_signature)) {
			throw ::std::exception("uninitialized iterator passed");//::std::string(__FUNCTION__) );
		}

		auto data = iterator->data;
		if (!data->is_valid) {
			throw ::std::exception("tried to iterate when it was in an invalid state");
		}

		datum_index::index_t nextIndex = iterator->next_index;
		long                 datum_size     = data->datum_size;
		uint                 pointer       = (uint)data->base_address + (datum_size * nextIndex);

		if (nextIndex > data->last_datum) {
			iterator->next_index = nextIndex;
			return nullptr;
		}

		datum_index nextDatum;

		while ( 1 )
		{
			nextDatum.salt = (*(short *)pointer);
			nextDatum.index = nextIndex++;
			if ( *(short *)pointer)
				break;

			pointer += datum_size;
			if ( nextIndex >= iterator->data->next_datum.index )
			{
				iterator->next_index = nextIndex;
				return 0;
			}
		}

		iterator->index = nextDatum;
		iterator->next_index = nextIndex;
		return (s_data_iterator  *)pointer;
	}

	static void datum_initialize(s_data_array *data, ushort *location) {

		memset(location, 0, data->datum_size);

		*location = data->next_datum.salt;

		if (!++data->next_datum.salt) {
			data->next_datum.salt = 0x8000u;
		}
	}

	//TODO: TEST AND VERIFY
	static datum_index datum_new_at_index(s_data_array *data, datum_index index) {

		if (data == nullptr || index.IsNull()) {
			return datum_index::null();
		}

		if ((index.index & 0x8000u) != 0 || index.index >= data->max_datum || index.salt == 0) {
			return datum_index::null();
		}

		auto current = reinterpret_cast<ushort *>((uint) data->base_address + (index.index * data->datum_size));

		if (*current) {
			return datum_index::null();
		}

		++data->next_index;

		if (index.index >= data->last_datum) {
			data->last_datum = index.index + 1;
		}

		datum_initialize(data, current);
		*current = index.salt;

		return index;
	}

	// creates a new element in [data] and returns the datum index
	//TODO: TEST AND VERIFY
	static datum_index datum_new(s_data_array *data) {

		auto        sz       = data->datum_size;
		auto        next_idx = data->next_index;
		datum_index result   = datum_index::null();
		auto        idx      = ((uint) data->base_address + sz * next_idx);
		if (next_idx < data->max_datum) {
			while (*(ushort *) idx) {
				++next_idx;
				idx += sz;
				if (next_idx >= data->max_datum)
					return result;
			}
			memset((void *) idx, 0, data->datum_size);
			*(ushort *) idx = data->next_datum.salt++;

			if (!data->next_datum.salt) {
				data->next_datum.salt = 0x8000u;
			}

			++data->next_datum.index;
			data->next_index = next_idx + 1;

			if (next_idx >= data->last_datum) {
				data->last_datum = next_idx + 1;
			}

			result.index = next_idx;
			result.salt  = *(signed __int16 *) idx << 16;
		}
		return result;
	}

	// Delete the data associated with the [index] handle in [data]
	//TODO: TEST AND VERIFY
	static void datum_delete(s_data_array *data, datum_index datum) {

		if (data == nullptr || datum.IsNull()) return;

		short *result = (short *) ((uint) data->base_address + datum.index * data->datum_size);
		int   v4; // edx

		if (datum.index < 0 || datum.index >= data->last_datum || !result) {
			return;
		}

		*result = 0;
		if (datum.index < data->next_index) {
			data->next_index = datum.index;
		}

		if (datum.index + 1 == data->last_datum) {
			v4 = data->datum_size;
			do {

				result -= v4;
				--data->last_datum;
			} while (data->last_datum > 0 && !*result);
		}
		--data->next_datum.index;
	}

	// Get the data associated with [index] from the [data] array
	// Returns nullptr if the handle is invalid
	//TODO: TEST AND VERIFY
	static void *datum_try_and_get(s_data_array *data, datum_index index) {
		if (!index.IsNull() && (index.index & 0x8000u) == 0 && (signed __int16) index.index < data->max_datum) {
			short *got = reinterpret_cast<short *>((uint) data->base_address + index.index * data->datum_size);
			if (got == nullptr || got == (short *)-1 || !*got) {
				return nullptr;
			}

			if (!index.salt || *(short *) got == index.salt)
				return reinterpret_cast<void *>((uint) data->base_address + index.index * data->datum_size);
		}
		return nullptr;
	}
};