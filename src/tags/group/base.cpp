#define WIN32_LEAN_AND_MEAN

//IDE thinks that windows.h is not used, but we use it for side-effects. "No Target Architecture" error.
// See: https://stackoverflow.com/questions/4845198/fatal-error-no-target-architecture-in-visual-studio
#include <windows.h>
#include <memory>
#include "tagreference.h"

namespace Yelo {
	namespace blam {
		// Clear the values of a tag reference so that it references no tag
		void tag_reference_clear(Yelo::tag_reference &reference) {
			// The engine's code will free (ie, YELO_FREE) the reference's name
			// when tag_block_delete_element (which is called by tag_unload) is ran

		::std::memset(reference.name, 0, Enums::k_max_tag_name_length + 1);
			reference.name_length = 0;
			reference.group_tag   = NONE;
			reference.tag_index   = datum_index::null();
		}

		void  tag_reference_set(tag_reference &reference, tag group_tag, const char *name) {
			//assert(group_tag == NONE || tag_group_get(group_tag));
			reference.group_tag = group_tag;

			size_t name_length = strlen(name);
			//YELO_ASSERT(name_length <= Enums::k_max_tag_name_length); // NOTE: engine does '<', but I'm pretty sure we want '<='

			if (reference.name != name)
				strcpy(reference.name, name);

			reference.name_length = name_length;
		}

		template <typename T>
		void tag_reference_set(tag_reference &reference, const char *name) {
			return tag_reference_set(reference, T::k_group_tag, name);
		}

		datum_index  tag_reference_try_and_get(const tag_reference *reference) {
			//
			// datum_index loaded_tag_index = tag_loaded(reference->group_tag, reference->name);
			//"tag reference \"%s\" and actual index do not match: is %08lX but should be %08lX",
			// 									  reference->name, reference->tag_index,
			// 									  loaded_tag_index

			return datum_index::null();
		}

		bool  tag_reference_resolve(_Inout_ tag_reference *reference) {

			bool success = false;
			// if (reference->group_tag != NONE && !is_null_or_empty(reference->name)) {
			// 	reference->tag_index = tag_load(reference->group_tag, reference->name, 0);
			// 	success = !reference->tag_index.IsNull();
			// } else
			// 	reference->tag_index = datum_index::null();

			return success;
		}

		// non-standard overload of the above resolve()
		bool tag_reference_resolve(_Inout_ tag_reference &reference, tag expected_group_tag) {
			bool success = false;
			if (reference.group_tag == expected_group_tag && !Yelo::is_null_or_empty(reference.name)) {
				// reference.tag_index = tag_load(reference.group_tag, reference.name, 0);
				// success = !reference.tag_index.IsNull();
			} else
				reference.tag_index = datum_index::null();

			return success;
		}

		template <typename T>
		bool tag_reference_resolve(_Inout_ tag_reference &reference) {
			return tag_reference_resolve(reference, T::k_group_tag);
		}

	};
};
