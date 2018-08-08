#include "object_lists.h"
#include "object_lists_structures.h"

namespace Yelo::blam{
	datum_index object_list_new() {
		datum_index list_index = datum_index::null(); //blam::datum_new(/*ObjectListHeader()*/);

		// if (!list_index.IsNull()) {
		// 	s_object_list_header_datum *list = object_list_get(list_index);
		//
		// 	list->count = 0;
		// 	list->first = datum_index::null();
		// }

		return list_index;
	}

	void object_list_delete(datum_index list_index) {
		// if (list_index.IsNull())
		// 	return;
		//
		// s_object_list_header_datum *list = object_list_get(list_index);
		//
		// // delete all reference datums used in this list
		// for (datum_index reference_index = list->first; !reference_index.IsNull();) {
		// 	s_list_object_reference_datum *reference           = object_list_object_get(reference_index);
		// 	datum_index                   next_reference_index = reference->next_reference;
		//
		// 	ListObjectReference().Delete(reference_index);
		//
		// 	// NOTE: engine doesn't make a copy of the next index before calling delete.
		// 	// meaning the engine assumes datum_delete() doesn't change any bits of the datum besides the header.
		// 	// we're not going to run with this assumption, so we make a copy before the delete() above
		// 	reference_index = next_reference_index;
		// }
		//
		// ObjectListHeader().Delete(list_index);
	}

	void object_lists_garbage_collect() {
		// NOTE: engine seems to use a for(;;) using data_next_index() on list headers
		// for (auto list : Yelo::Scripting::ObjectListHeader()) {
		// 	if (list->count == 0)
		// 		object_list_delete(list.index);
		// }
	}
}
