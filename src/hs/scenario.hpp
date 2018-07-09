#pragma once


#include "../tags/group/markup.h"
#include "../tags/group/base.h"
#include "hs.h"
#include "../cseries/base.h"
#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo {
	namespace TagGroups {
		struct hs_scenario_data_base {

			struct _script {
				Yelo::Enums::hs_script_type type;
				Yelo::Enums::hs_type        return_type;
				datum_index                 root_expression_index;
			};

			struct _global {
				Enums::hs_type type;
				unsigned short : 16;
				unsigned long  : 32;
				datum_index initialization_expression_index;
			};

			tag_string name;

			union {
				long:8 * sizeof(long) * 15; // 60
				_script script;
				_global global;
			};
		};

		struct hs_script : hs_scenario_data_base {};
		static_assert(sizeof(hs_script) == 0x5C);

		struct hs_global_internal : hs_scenario_data_base {};
		static_assert(sizeof(hs_global_internal) == 0x5C);

		struct hs_tag_reference {
			long:8 * sizeof(long) * 6; // 24
			tag_reference reference;
		};
		static_assert(sizeof(hs_tag_reference) == 0x28);

		struct hs_source_file {
			tag_string name;
			tag_data   source;
		}; static_assert(sizeof(hs_source_file) == 0x34);
	};

	namespace blam {
		bool __cdecl hs_scenario_postprocess(bool for_runtime);
	};
};
