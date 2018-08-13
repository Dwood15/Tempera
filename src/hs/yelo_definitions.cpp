#pragma once

#include "yelo_definitions.h"

namespace Yelo::Scripting {
	void c_hs_library::Initialize() {}

	void c_hs_library::Dispose() {}

	void c_hs_library::FinishRegisteringBlamFunctions() {
		// g_functions_yelo_start_index = g_functions.size();
	}

	void c_hs_library::FinishRegisteringBlamGlobals() {
		// g_external_globals_yelo_start_index = g_external_globals.size();
	}

	size_t c_hs_library::GetYeloFunctionsCount() {
		return g_functions.size() + g_functions_yelo_start_index;
	}

	size_t c_hs_library::GetYeloGlobalsCount() {
		return g_external_globals.size() + g_external_globals_yelo_start_index;
	}

	const ::std::vector<hs_function_definition *> &c_hs_library::GetFunctionsTable(size_t *yelo_start_index) {
		if (yelo_start_index)
			*yelo_start_index = g_functions_yelo_start_index;

		return g_functions;
	}

	const ::std::vector<hs_global_definition *> &c_hs_library::GetExternalGlobals(size_t *yelo_start_index) {
		if (yelo_start_index)
			*yelo_start_index = g_external_globals_yelo_start_index;

		return g_external_globals;
	}

	hs_function_definition *c_hs_library::GetFunction(short function_index) {
		YELO_ASSERT(function_index >= 0 && (size_t) function_index < g_functions.size());

		auto *hs_function = g_functions[function_index];

		YELO_ASSERT(hs_function);
		return hs_function;
	}

	hs_global_definition *c_hs_library::GetGlobal(short global_index) {
		YELO_ASSERT(global_index >= 0 && (size_t) global_index < g_external_globals.size());

		auto *hs_global = g_external_globals[global_index];

		YELO_ASSERT(hs_global);
		return hs_global;
	}

	hs_function_definition *c_hs_library::GetYeloFunction(short function_index) {
		YELO_ASSERT(function_index >= 0);

		return GetFunction(static_cast<short>(g_functions_yelo_start_index) + function_index);
	}

	hs_global_definition *c_hs_library::GetYeloGlobal(short global_index) {
		YELO_ASSERT(global_index >= 0);

		return GetGlobal(static_cast<short>(g_external_globals_yelo_start_index) + global_index);
	}

	template <typename T>
	void c_hs_library::RegisterLibraryElement(::std::vector<T *> &vector, size_t &next_register_index, T &element) {
		// since we support 'fixups' that map directly to a specific index, we first have to try and find an
		// unused slot we can put the desired element at.
		if (next_register_index < vector.size()) {
			bool      added = false;
			for (auto iter  = vector.begin() + next_register_index, end = vector.end(); !added && iter != end; ++iter, ++next_register_index) {
				if (*iter != nullptr) {
					continue;
				}

				added = true;
				*iter = &element;
			}

			// we found an unused slot and added it there, we can return.
			// else, there are no more free slots, and have to go back to regular 'push_back' adding
			if (added) {
				return;
			}
		}

		// we should only be adding when there are no more free slots in the vector
		YELO_ASSERT(next_register_index == vector.size());

		vector.push_back(&element);
		next_register_index++;
	}

	void c_hs_library::RegisterFunction(hs_function_definition &definition) {
		// RegisterLibraryElement(g_functions, g_functions_next_register_index, definition);
	}

	void c_hs_library::RegisterGlobal(hs_global_definition &definition) {
		// YELO_ASSERT(g_external_globals.size() < Enums::k_hs_external_globals_count_upgrade);
		//
		// RegisterLibraryElement(g_external_globals, g_external_globals_next_register_index, definition);
	}
};
