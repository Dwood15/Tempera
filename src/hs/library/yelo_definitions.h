#pragma once

#include "../structures.h"

#include <cstddef>

namespace Yelo::Scripting {
	struct hs_function_definition;
	struct hs_global_definition;

	struct s_hs_library_fixup {
		size_t index;
		union {
			void *address;

			hs_function_definition *function;
			hs_global_definition   *global;
		};
	};

	class c_hs_library {

		static size_t                                g_functions_yelo_start_index; ///< Starting index of OS-defined functions
		static size_t                                g_functions_next_register_index;
		const ::std::vector<hs_function_definition *> g_functions              =::std::vector<hs_function_definition *>(1024); //k_hs_script_functions_count_upgrade);

		static size_t                                     g_external_globals_yelo_start_index; ///< Starting index of OS-defined globals
		static size_t                                     g_external_globals_next_register_index;
		static::std::vector<hs_global_definition *> g_external_globals; //(Yelo::Enums::k_hs_external_globals_count_upgrade);
	public:

		void Initialize() {}

		void Dispose() {}

		void FinishRegisteringBlamFunctions() {
			// g_functions_yelo_start_index = g_functions.size();
		}

		void FinishRegisteringBlamGlobals() {
			// g_external_globals_yelo_start_index = g_external_globals.size();
		}

		size_t GetYeloFunctionsCount() {
			return g_functions.size() + g_functions_yelo_start_index;
		}

		size_t GetYeloGlobalsCount() {
			return g_external_globals.size() + g_external_globals_yelo_start_index;
		}

		const::std::vector<hs_function_definition *> &GetFunctionsTable(size_t *yelo_start_index = nullptr) {
			if (yelo_start_index)
				*yelo_start_index = g_functions_yelo_start_index;

			return g_functions;
		}

		static const::std::vector<hs_global_definition *> &GetExternalGlobals(size_t *yelo_start_index = nullptr) {
			if (yelo_start_index)
				*yelo_start_index = g_external_globals_yelo_start_index;

			return g_external_globals;
		}

		hs_function_definition *GetFunction(short function_index) {
			YELO_ASSERT(function_index>=0 && (size_t)function_index<g_functions.size());

			auto* hs_function = g_functions[function_index];

			YELO_ASSERT(hs_function);
			return hs_function;
		}

		hs_global_definition *GetGlobal(short global_index) {
			YELO_ASSERT(global_index>=0 && (size_t)global_index<g_external_globals.size());

			auto* hs_global = g_external_globals[global_index];

			YELO_ASSERT(hs_global);
			return hs_global;
		}

		hs_function_definition *GetYeloFunction(short function_index) {
			YELO_ASSERT(function_index>=0);

			return GetFunction(static_cast<short>(g_functions_yelo_start_index)+function_index);
		}

		hs_global_definition *GetYeloGlobal(short global_index) {
			YELO_ASSERT(global_index>=0);

			return GetGlobal(static_cast<short>(g_external_globals_yelo_start_index) + global_index);
		}

		template<typename T>
		void RegisterLibraryElement(::std::vector<T*>& vector, size_t& next_register_index, T& element) {
			// since we support 'fixups' that map directly to a specific index, we first have to try and find an
			// unused slot we can put the desired element at.
			if (next_register_index < vector.size()) {
				bool added = false;
				for (auto iter = vector.begin() + next_register_index, end = vector.end(); !added && iter != end; ++iter, ++next_register_index) {
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

		void RegisterFunction(hs_function_definition &definition) {
			// RegisterLibraryElement(g_functions, g_functions_next_register_index, definition);
		}

		void RegisterGlobal(hs_global_definition &definition) {
			// YELO_ASSERT(g_external_globals.size() < Enums::k_hs_external_globals_count_upgrade);
			//
			// RegisterLibraryElement(g_external_globals, g_external_globals_next_register_index, definition);
		}

	private:
		template<typename T>
		void RegisterLibraryFixup(::std::vector<T*>& vector, size_t fixup_index, T* element) {
			//YELO_ASSERT(element);

			if (fixup_index >= vector.size()) {
				size_t new_size = vector.size();
				new_size += (vector.size() - fixup_index) + 1;
				vector.resize(new_size);
			}

			vector[fixup_index] = element;
		}

		void RegisterFunctionFixup(s_hs_library_fixup &fixup) {
			//RegisterLibraryFixup(g_functions, fixup.index, fixup.function);
		}

		void RegisterGlobalFixup(s_hs_library_fixup &fixup) {
			RegisterLibraryFixup(g_external_globals, fixup.index, fixup.global);
		}

	public:
		template <rsize_t k_array_size>
		void RegisterFunctionFixups(s_hs_library_fixup (&fixups)[k_array_size]) {
			for (rsize_t x = 0; x < k_array_size; x++) {
				RegisterFunctionFixup(fixups[x]);
			}
		}

		template <rsize_t k_array_size>
		void RegisterGlobalFixups(s_hs_library_fixup (&fixups)[k_array_size]) {
			for (rsize_t x = 0; x < k_array_size; x++) {
				RegisterGlobalFixup(fixups[x]);
			}
		}
	};

	::std::vector<hs_global_definition *> c_hs_library::g_external_globals =::std::vector<hs_global_definition *>(896);
};
