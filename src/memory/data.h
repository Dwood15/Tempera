#pragma once

#include <rpc.h>
#include <exception>
#include <addlog.h>
#include <cstring>
#include "datum_index.h"
#include "data_base.h"
#include "data_iterator.h"
#include "../CurrentEngine.h"

//TODO: VERIFY EACH AND EVERY DOCALL:
//Function call may be different convention between sapien and haloce
//Sapien may not have function but haloce does.
namespace Yelo {

	namespace blam {
		using namespace Yelo::Memory;

		// [[deprecated]]
		// static s_data_array *data_new(const char *name, long maximum_count, size_t datum_size);

		//Intended to be a complete replacement for the in-game data_new :)
		//TODO: TEST + confirm works. _Looks_ functionally similar to the original data_new.
		template <typename T, int max_count>
		static s_data_array *data_new(const char *name);

		static void data_dispose(s_data_array *data);

		//This replaces the engine version of data_delete_all
		//TODO: TEST
		static void data_delete_all(s_data_array *data);

		static void data_make_valid(s_data_array *data);

		static void data_make_invalid(s_data_array *data);

		static datum_index data_next_index(s_data_array *data, datum_index cursor);

		static void data_iterator_new(s_data_iterator &iterator, s_data_array *data);

		static void *data_iterator_next(s_data_iterator &iterator);

		static void datum_initialize(s_data_array *data, ushort *location);

		//TODO: TEST AND VERIFY
		static datum_index datum_new_at_index(s_data_array *data, datum_index index);

		// creates a new element in [data] and returns the datum index
		//TODO: TEST AND VERIFY
		static datum_index datum_new(s_data_array *data);

		// Delete the data associated with the [index] handle in [data]
		//TODO: TEST AND VERIFY
		static void datum_delete(s_data_array *data, datum_index datum);

		// Get the data associated with [index] from the [data] array
		//TODO: TEST AND VERIFY
		template <typename T>
		static void *datum_get(s_data_array *data, datum_index index);

		// Get the data associated with [index] from the [data] array
		// Returns nullptr if the handle is invalid
		//TODO: TEST AND VERIFY
		static void *datum_try_and_get(s_data_array *data, datum_index index);
	};
};
