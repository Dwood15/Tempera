#pragma once

#include <cstddef>
#include "structures.h"


namespace Yelo::Scripting {
	struct hs_global_definition;
	struct hs_function_definition;

	class c_hs_library {
		size_t                                g_functions_yelo_start_index; ///< Starting index of OS-defined functions
		size_t                                g_functions_next_register_index;
		const ::std::vector<Yelo::Scripting::hs_function_definition *> g_functions              =::std::vector<hs_function_definition *>(1024); //k_hs_script_functions_count_upgrade);

		size_t                                     g_external_globals_yelo_start_index; ///< Starting index of OS-defined globals
		size_t                                     g_external_globals_next_register_index;
		std::vector<hs_global_definition *> g_external_globals; //(Yelo::Enums::k_hs_external_globals_count_upgrade);
	public:

		c_hs_library() {
			g_external_globals.reserve(896);
		}
		void Initialize();

		void Dispose();

		void FinishRegisteringBlamFunctions();

		void FinishRegisteringBlamGlobals();

		size_t GetYeloFunctionsCount();

		size_t GetYeloGlobalsCount();

		const::std::vector<hs_function_definition *> &GetFunctionsTable(size_t *yelo_start_index = nullptr);

		const::std::vector<hs_global_definition *> &GetExternalGlobals(size_t *yelo_start_index = nullptr);

		hs_function_definition *GetFunction(short function_index);

		hs_global_definition *GetGlobal(short global_index);

		hs_function_definition *GetYeloFunction(short function_index);

		hs_global_definition *GetYeloGlobal(short global_index);

		template<typename T>
		void RegisterLibraryElement(::std::vector<T*>& vector, size_t& next_register_index, T& element);

		void RegisterFunction(hs_function_definition &definition);

		void RegisterGlobal(hs_global_definition &definition);

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

		template<typename T>
		void RegisterFunctionFixup(T &fixup) {
			//RegisterLibraryFixup(g_functions, fixup.index, fixup.function);
		}

		template<typename T>
		void RegisterGlobalFixup(T &fixup) {
			RegisterLibraryFixup(g_external_globals, fixup.index, fixup.global);
		}

	public:
		template <typename T, rsize_t k_array_size>
		void RegisterFunctionFixups(T (&fixups)[k_array_size]) {
			for (rsize_t x = 0; x < k_array_size; x++) {
				RegisterFunctionFixup(fixups[x]);
			}
		}

		template <typename T, rsize_t k_array_size>
		void RegisterGlobalFixups(T (&fixups)[k_array_size]) {
			for (rsize_t x = 0; x < k_array_size; x++) {
				RegisterGlobalFixup(fixups[x]);
			}
		}
	};
};
