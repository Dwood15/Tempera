#pragma once

#include "../memory/datum_index.h"
#include "../memory/array.h"
#include "object_lists_structures.h"

namespace Yelo {
	namespace Scripting {

		typedef Yelo::Memory::DataArray<s_object_list_header_datum, 48> object_list_header_data_t;

		typedef Yelo::Memory::DataArray<s_list_object_reference_datum, 128> list_object_reference_data_t;

		extern object_list_header_data_t &ObjectListHeader();

		list_object_reference_data_t &ListObjectReference();
	};
}

namespace Yelo::blam {
	using namespace Scripting;

	extern s_object_list_header_datum *object_list_get(datum_index list_index);

	extern Scripting::s_list_object_reference_datum *object_list_object_get(datum_index list_reference_index);

	extern datum_index object_list_new();

	static void object_list_delete(datum_index list_index);

	static void object_lists_garbage_collect();

	// Add [object_index] to [list_index]
	static void object_list_add(datum_index list_index, datum_index object_index) {}

	// Get the next object_index in a list of objects
	// [list_reference]:
	// in  - handle to the current s_list_object_reference_datum in the object_list we're referencing
	// out - handle to the next s_list_object_reference_datum
	static datum_index object_list_get_next(datum_index list_index, _Inout_ datum_index &list_reference) {
		if (!list_reference.IsNull()) {
			s_list_object_reference_datum *object_reference = ListObjectReference()[list_reference];
			list_reference = object_reference->next_reference;

			return object_reference->object_index;
		}

		return datum_index::null();
	}
	// Get the object_index of the first object in a list of objects
	// [list_reference] - handle to the object_list's first s_list_object_reference_datum
	static datum_index object_list_get_first(datum_index list_index, _Out_ datum_index &list_reference) {
		if (!list_index.IsNull()) {
			s_object_list_header_datum *list = object_list_get(list_index);
			list_reference = list->first;

			return object_list_get_next(list_index, list_reference);
		}

		return datum_index::null();
	}

	static void object_list_add_reference(datum_index list_index) {
		if (list_index.IsNull())
			return;

		s_object_list_header_datum *list = object_list_get(list_index);

		list->reference_count++;
	}

	static void object_list_remove_reference(datum_index list_index) {
		if (list_index.IsNull())
			return;

		s_object_list_header_datum *list = object_list_get(list_index);

		list->reference_count--;
	}

	static short object_list_count(datum_index list_index) {
		if (list_index.IsNull())
			return 0;

		s_object_list_header_datum *list = object_list_get(list_index);

		return list->count;
	}
};
