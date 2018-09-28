#pragma once

namespace Yelo::TagGroups {
	struct s_effect_postprocess_collection_script_variable {
		tag_string script_variable_name;
		tag_string exposed_parameter_name;

		struct {
			void *exposed_parameter; //void* to avoid a problematic include

			byte pad_type12[12];
		}          runtime;
	};
	STAT_ASSERT(s_effect_postprocess_collection_script_variable, 0x50);

	struct s_effect_postprocess_collection_effect {
		tag_reference                                                   effect;
		tag_string                                                      name;
		Yelo::TagBlock<s_effect_postprocess_collection_script_variable> script_variables;
		byte                                                            pad_type12[12];
	};
	STAT_ASSERT(s_effect_postprocess_collection_effect, 0x48);

	struct s_effect_postprocess_collection {
		enum { k_group_tag = 'efpc' };

		byte                                                         pad_type12[12];
		Yelo::TagBlock<tag_reference>                                shaders;
		Yelo::TagBlock<s_effect_postprocess_collection_effect>       effects;
		Yelo::TagBlock<s_effect_postprocess_generic_effect_instance> effect_instances;
	};
	STAT_ASSERT(s_effect_postprocess_collection, 0x30);
};
