/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <precompile.h>

namespace Yelo::Memory {
	// C++ wrapper for blamlib's s_dynamic_array
	template <typename DatumT>
	class DynamicArray {
		size_t element_size;
		long  count;
		DatumT *elements;

	public:
		long GetCount() const { return count; }

		const DatumT *operator [](long index) const {
			if (index < 0 || index >= count)
				return nullptr;

			return &reinterpret_cast<DatumT *>(elements)[index];
		}

		DatumT *operator [](long index) {
			if (index < 0 || index >= count)
				return nullptr;

			return &reinterpret_cast<DatumT *>(elements)[index];
		}
	};

	// I've only seen gbx code use this structure format...
	// Therefore, I declare ye' GbxArray!
	// Guess they didn't want to change dynamic_array to support a 'capacity'
	template <typename DatumT>
	struct GbxArray {
	public:
		DatumT *elements;
		long  count;
		long  capacity;

	public:
		long GetCount() const { return count; }

		long GetCapacity() const { return capacity; }

		const DatumT *operator [](long index) const {
			if (index < 0 || index >= count)
				return nullptr;

			return &reinterpret_cast<DatumT *>(elements)[index];
		}

		DatumT *operator [](long index) {
			if (index < 0 || index >= count)
				return nullptr;

			return &reinterpret_cast<DatumT *>(elements)[index];
		}

	};
};
