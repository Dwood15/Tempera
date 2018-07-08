#pragma once

#include <cstring>
#include <rpc.h>
#include <exception>
#include <addlog.h>
#include "datum_index.h"
#include "data_base.h"
#include "../ce_base_types.h"
#include "../CurrentEngine.h"

static constexpr Yelo::datum_index::salt_t k_datum_index_salt_msb = 1U << (15);
//TODO: VERIFY EACH AND EVERY DOCALL:
//Function call may be different convention between sapien and haloce
//Sapien may not have function but haloce does.
namespace Yelo {
	namespace Memory {
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

			datum_index::salt_t GetInitialSalt() const {
				datum_index::salt_t result = 0;
				// NOTE: engine uses strncpy
				std::memcpy(&result, this->name, sizeof(result));

				result |= k_datum_index_salt_msb;

				return result;
			}

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

		static_assert(sizeof(s_data_array) == 0x38);

		struct s_data_iterator {
			s_data_array *data;
			short        next_index;
			short        finished_flag; // actually alignment, unused by the engine
			datum_index  index;
			tag          signature;

			//////////////////////////////////////////////////////////////////////////
			// stl end() hacks
			enum { k_end_hack_signature = 'hack' };

			inline void SetEndHack() { signature = k_end_hack_signature; }

			inline bool IsEndHack() const { return signature == k_end_hack_signature; }

			// HACK: don't use this unless the s_data_iterator was created in the OS codebase!
			// engine's iterator_new doesn't initialize 'finished_flag' and we use it for end() hacks
			bool operator !=(const s_data_iterator &other) const {
				auto last_datum = this->data->last_datum;

				if (other.IsEndHack()) {
					return !this->finished_flag;
				}

				if (this->IsEndHack()) {
					return !other.finished_flag;
				}

				return this->index != other.index;
			}
		};

		static_assert(sizeof(s_data_iterator) == 0x10);

		s_data_array *DataNewAndMakeValid(const char *name, long maximum_count, size_t datum_size) {
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

	};

	namespace blam {
		using namespace Yelo::Memory;

		void data_verify(const Memory::s_data_array *data) {}

		void data_delete_all(Memory::s_data_array *data);

		[[deprecated]]
		s_data_array *data_new(const char *name, long maximum_count, size_t datum_size) {
			static uintptr_t FUNCTION = CurrentEngine.getFunctionBegin("data_new");

			__asm mov      ebx, datum_size
			return calls::DoCall<Convention::m_cdecl, s_data_array *, long, const char *>(FUNCTION, maximum_count, name);
		}

		//Intended to be a complete replacement for the in-game data_new :)
		//TODO: TEST + confirm works. _Looks_ functionally similar to the original data_new.
		template <typename T, int max_count>
		s_data_array *data_new(const char *name) {
			constexpr auto size = static_cast<short>(sizeof(T));

			auto allocd = GlobalAlloc(0, max_count * size + sizeof(s_data_array));

			auto newData = reinterpret_cast<s_data_array *>(allocd);

			if (newData) {
				memset(newData, 0, sizeof(s_data_array));
				strncpy(newData->name, name, 31u);
				newData->max_datum    = size * max_count;
				newData->datum_size   = size;
				newData->signature    = 'd@t@';
				newData->is_valid     = 0;
				newData->base_address = newData + sizeof(s_data_array);
			}

			return newData;
		}

		void data_dispose(s_data_array *data) {
			if (data != nullptr) {
				data_verify(data);
				reinterpret_cast<s_data_array *>(GlobalFree(data));
			}
		}

		//This replaces the engine version of data_delete_all
		//TODO: TEST
		void data_delete_all(s_data_array *data) {
			data->last_datum       = 0;
			data->next_datum.index = 0;
			data->next_index       = 0;
			strncpy((char *) &data->next_datum.salt, data->name, 2u);
			data->next_datum.salt |= 0x8000u;
			if (data->max_datum > 0) {
				return; // we're done here!
			}

			for (short i = 0; i < data->max_datum; i++) {
				auto current = i * data->datum_size;
				*(byte *) (&data->base_address)[current] = 0;
			}
		}

		void data_make_valid(s_data_array *data) {
			// data_verify(data);
			data->is_valid = true;
			data_delete_all(data);
		}

		void data_make_invalid(s_data_array *data) {
			// data_verify(data);
			data->is_valid = false;
		}

		datum_index data_next_index(s_data_array *data, datum_index cursor) {
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

		void data_iterator_new(s_data_iterator &iterator, s_data_array *data) {
			data_verify(data);

			if (!data->is_valid) {
				throw std::exception("invalid data array passed to " __FUNCTION__);
			}
			iterator.data          = data;
			iterator.next_index    = 0;
			iterator.finished_flag = false;
			iterator.index         = datum_index::null();
			iterator.signature     = reinterpret_cast<uintptr_t>(data) ^ Enums::k_data_iterator_signature;
		}

		void *data_iterator_next(s_data_iterator &iterator) {
			if (!(iterator.signature == (reinterpret_cast<uintptr_t>(iterator.data) ^ Enums::k_data_iterator_signature))) {
				throw std::exception("uninitialized iterator passed to " __FUNCTION__);
			}

			const s_data_array *data = iterator.data;
			data_verify(data);
			if (!data->is_valid) {
				//std::str(data->name)
				throw std::exception("tried to iterate when it was in an invalid state ");
			}

			datum_index::index_t absolute_index = iterator.next_index;
			long                 datum_size     = data->datum_size;
			byte                 *pointer       = reinterpret_cast<byte *>(data->base_address) + (datum_size * absolute_index);

			for (short last_datum  = data->last_datum; absolute_index < last_datum; pointer += datum_size, absolute_index++) {
				auto datum = reinterpret_cast<const s_datum_base *>(pointer);

				if (!datum->IsNull()) {
					iterator.next_index = absolute_index + 1;
					iterator.index      = datum_index::Create(absolute_index, datum->GetHeader());
					return pointer;
				}
			}
			iterator.next_index    = absolute_index; // will equal last_datum at this point
			iterator.finished_flag = true;
			iterator.index         = datum_index::null();
			return nullptr;
		}

		void datum_initialize(s_data_array *data, ushort *location) {

			memset(location, 0, data->datum_size);

			*location = data->next_datum.salt;

			if (!++data->next_datum.salt) {
				data->next_datum.salt = 0x8000u;
			}

		}

		//TODO: TEST AND VERIFY
		datum_index datum_new_at_index(s_data_array *data, datum_index index) {

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
		datum_index datum_new(s_data_array *data) {

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
		void datum_delete(s_data_array *data, datum_index datum) {

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
		//TODO: TEST AND VERIFY
		template <typename T>
		void *datum_get(s_data_array *data, datum_index index) {

			if (sizeof(T) != data->datum_size) {
				Print(false, "Datum_get for object size: %d doesn't match engine size: %d", sizeof(T), data->datum_size);
				throw std::exception("Datum get mismatch! See debug log.");
			}

			T *object_array = reinterpret_cast<T *>(data->base_address); // edx

			short item = *(short *) &object_array[index.index]; // cx

			if (index.index < 0 || index.index >= data->last_datum) {
				return 0;
			}

			if ((object_array, item, !item)) {
				return 0;
			}

			return &object_array[index.index];
		}

		// Get the data associated with [index] from the [data] array
		// Returns nullptr if the handle is invalid
		//TODO: TEST AND VERIFY
		void *datum_try_and_get(s_data_array *data, datum_index index) {
			if (!index.IsNull() && (index.index & 0x8000u) == 0 && (signed __int16) index.index < data->max_datum) {
				short *got = reinterpret_cast<short *>((uint) data->base_address + index.index * data->datum_size);
				if (*got) {
					if (!index.salt || *(short *) got == index.salt)
						return reinterpret_cast<void *>((uint) data->base_address + index.index * data->datum_size);
				}
			}
			return nullptr;
		}
	};
};
