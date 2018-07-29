#pragma once

#include <enums/hs_enums.h>
#include "macros_generic.h"
#include "hs.h"

#include "enums/hs_enums.h"

namespace Yelo::Scripting {
	typedef void (__cdecl *proc_hs_parse)(long function_index, datum_index expression_index);

	typedef void (__cdecl *proc_hs_evaluate)(long function_index, datum_index thread_index, bool initialize_stack);





	// halo script function definition
	struct hs_function_definition {
		Yelo::Enums::hs_type return_type;
		unsigned short       yelo_flags; // padding in halo, special flags in project yellow
		const char           *name;
		proc_hs_parse        parse;
		proc_hs_evaluate     evaluate;
		const char           *info;
		const char           *param_info;
		unsigned short       access;
		short                paramc;
		// #pragma warning( push )
		// #pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
		// don't access directly, use GetParameter
		Enums::hs_type       params[];
		// #pragma warning( pop )

		Enums::hs_type GetParameter(short index) {
			// assert(index >= 0 && index<paramc);

			return params[index];
		}

		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		typedef Enums::hs_type       *iterator;
		typedef const Enums::hs_type *const_iterator;

		const_iterator begin() const { return params; }

		iterator begin() { return params; }

		const_iterator const_begin() const { return params; }

		const_iterator const_begin() { return params; }

		const_iterator end() const { return params + paramc; }

		iterator end() { return params + paramc; }

		const_iterator const_end() const { return params + paramc; }

		const_iterator const_end() { return params + paramc; }

	};

	STAT_ASSERT(hs_function_definition, 0x1C); // size doesn't include [params]

	// halo script accessible value
	struct hs_global_definition {
		const char     *name;
		Enums::hs_type type;
		unsigned short flags; // padding in halo, special flags in project yellow
		union {
			void *address;

			s_hs_value_union value;
		};
		unsigned short access;
		unsigned short padA;
	};
	STAT_ASSERT(hs_global_definition, 0x10);

	struct hs_syntax_node : Memory::s_datum_base {
		union {
			Enums::hs_type constant_type;
			short          function_index;
			short          script_index;
		};
		Enums::hs_type type;

		union {
			unsigned short flags;
			short          pointer_type;
		};

		datum_index next_expression;
		long        source_offset;
		union {
			void *address;

			s_hs_value_union value;
		};

		bool IsScriptCall() const {
			return TEST_FLAG(flags, Flags::_hs_syntax_node_script_index_bit);
		}

		bool IsGarbageCollectable() const {
			return !TEST_FLAG(flags, Flags::_hs_syntax_node_dont_gc_bit);
		}
	};

	STAT_ASSERT(hs_syntax_node, 0x14);

	typedef Memory::DataArray<hs_syntax_node, Yelo::Enums::k_maximum_hs_syntax_nodes_per_scenario, Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade> hs_syntax_data_t;

	struct s_hs_globals_datum : Yelo::Memory::s_datum_base {
		unsigned short yelo_flags;

		Yelo::Scripting::s_hs_value_union value;
	};

	typedef Memory::DataArray<s_hs_globals_datum, Yelo::Enums::k_maximum_number_of_hs_globals> hs_globals_data_t;


	STAT_ASSERT(s_hs_globals_datum, 0x8);

	struct s_hs_runtime_globals {
		bool enabled;
		unsigned char : 8;
		unsigned short : 16;
		short          executing_thread_index;
		bool           run_game_scripts;
		unsigned char : 8;
	};

	STAT_ASSERT(s_hs_runtime_globals, 0x8);

	hs_syntax_data_t &HsSyntax();
};

