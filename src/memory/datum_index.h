/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <macros_generic.h>

namespace Yelo {
	constexpr uint max_uint = static_cast<uint>(-1);

	/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
	struct datum_index {
		typedef short index_t;
		typedef short salt_t;

		union {
			uint handle;
			struct {
				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>
				/// 	absolute index Note: This is actually suppose to be a unsigned short but it's not until
				/// 	Halo 3 where data starts breaking past the 0x7FFF cap, so for Halo 1 and Halo 2 projects
				/// 	this should still hold safe. Maybe I'll update the code to unsigned short one day...
				/// </summary>
				index_t index;
				/// <summary>	salted index, gives the handle a certain uniqueness. </summary>
				salt_t  salt;
			};
		};

		static constexpr datum_index null() {
			return {{max_uint}};
		}

		bool IsNull() const { return max_uint == handle; }

		static datum_index Create(index_t index, salt_t salt) {
			datum_index result;
			result.handle = (static_cast<uint>(salt) << 16) | index;
			return result;
		}

		// [header] should be a pointer to the start of a datum instance in a data array
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new datum_index based on an absolute index and datum pointer. </summary>
		///
		/// <param name="index"> 	Zero-based index of the datum. </param>
		/// <param name="header">	[in] should be a pointer to the start of a datum instance in a data array </param>
		///
		/// <returns>	[datum_index::null] if this fails </returns>
		static datum_index Create(index_t index, const void *header) {
			if (header == nullptr) return null();

			return Create(index, *reinterpret_cast<const salt_t*>(header));
		}

		//OVERRIDE_OPERATOR_CAST_THIS(uint);

		datum_index& operator = (uint arg) {
			this->handle = arg;
			return *this;
		}

		inline bool operator ==(const datum_index& rhs) const { return this->handle == rhs.handle; }
		inline bool operator !=(const datum_index& rhs) const { return this->handle != rhs.handle; }

	private:

		inline bool operator ==(const uint& rhs) const { return this->handle == rhs; }
		inline bool operator !=(const uint& rhs) const { return this->handle != rhs; }
	};

	STAT_ASSERT(datum_index, 0x4);

	template <typename T>
	struct data_header {
		char              name[32];
		short             max;      // Max number of <things> possible
		short             size;      // Size of each <thing> class instance
		bool              is_valid;
		bool              identifier_zero_invalid;
		short             padding;
		unsigned long     signature; // d@t@
		short             next_index;
		short             last_index;
		Yelo::datum_index next;         // the next <thing> to be initialized
		T                 *first;         // Pointer to the first <thing> class instance
	};

	STAT_ASSERT(data_header<void>, 0x38);

#define pad_datum_index unsigned short : 16; unsigned short : 16
#define DATUM_INDEX_TO_IDENTIFIER(datum)      (datum & 0xFFFF0000)
#define DATUM_INDEX_TO_ABSOLUTE_INDEX(datum)   (datum & 0x0000FFFF)
};
