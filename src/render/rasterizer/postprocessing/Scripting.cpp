#pragma once

#include "Scripting.h"
#include "../../../math/colors.h"
#include "../../../cseries/yelo_base.h"
#include "PostProcessing.h"
#include "Generic/Internal/c_system_internal.hpp"

namespace Yelo {
	namespace Rasterizer {
		namespace PostProcessing {
			namespace Scripting {
				void *HS_Load() {
					LoadSystem();

					return NULL;
				}

				void *HS_Unload() {
					UnloadSystem();

					return NULL;
				}

				namespace Globals {
					bool Enabled_External() {
						return false;
					}

					bool Enabled_Internal() {
						return true;
					}

					bool Enabled_MotionBlur() {
						return false;
					}

					bool Enabled_FXAA() {
						return true;
					}

					real MotionBlur_Amount() {
						return 0.0f;
					}
				};

				namespace Bloom {
					void *HS_BloomSetSize(void **arguments) {
						struct s_arguments {
							real size;
							real interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						// PostProcessing::Bloom::c_system_bloom::Instance().SetBloomSize(args->size, args->interp_time);

						return NULL;
					}

					void *HS_BloomSetExposure(void **arguments) {
						struct s_arguments {
							real exposure;
							real interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						// PostProcessing::Bloom::c_system_bloom::Instance().SetBloomExposure(args->exposure, args->interp_time);

						return NULL;
					}

					void *HS_BloomSetMixAmount(void **arguments) {
						struct s_arguments {
							real mix_amount;
							real interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						// PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMixAmount(args->mix_amount, args->interp_time);

						return NULL;
					}

					void *HS_BloomSetMinimumColor(void **arguments) {
						struct s_arguments {
							real_rgb_color minimum_color;
							real           interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						// PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMinimumColor(args->minimum_color, args->interp_time);

						return NULL;
					}

					void *HS_BloomSetMaximumColor(void **arguments) {
						struct s_arguments {
							real_rgb_color maximum_color;
							real           interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						// PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMaximumColor(args->maximum_color, args->interp_time);

						return NULL;
					}
				};
				namespace Internal {
					void *HS_GetEffectInstanceIndexByName(void **arguments) {
						struct s_arguments {
							const char *test_string;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.uint32 = Generic::Internal::c_system_internal::Instance().GetEffectInstanceIndexByName(args->test_string);

						return result.pointer;
					}

					void *HS_SetEffectInstanceActive(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							bool active_bool;
							ushort : 16;
							byte   : 8;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectInstanceActive(args->effect_index, args->active_bool);

						return NULL;
					}

					void *HS_SetEffectInstanceFade(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							real fade_start;
							real fade_end;
							real fade_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectInstanceFade(args->effect_index, args->fade_start,
																													  args->fade_end, args->fade_time);

						return NULL;
					}

					void *HS_GetEffectInstanceCurrentFade(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.real = Generic::Internal::c_system_internal::Instance().GetEffectInstanceCurrentFade(args->effect_index);

						return result.pointer;
					}

					void *HS_GetEffectInstanceFadeDirection(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.int16 = Generic::Internal::c_system_internal::Instance().GetEffectInstanceFadeDirection(args->effect_index);

						return result.pointer;
					}

					void *HS_GetEffectIndexByName(void **arguments) {
						struct s_arguments {
							const char *test_string;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.uint32 = Generic::Internal::c_system_internal::Instance().GetEffectIndexByName(args->test_string);

						return result.pointer;
					}

					void *HS_GetEffectIsValid(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.boolean = Generic::Internal::c_system_internal::Instance().GetEffectIsValid(args->effect_index);

						return result.pointer;
					}

					void *HS_GetEffectShaderVariableIndexByName(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							const char *test_string;
						}          *args = CAST_PTR(s_arguments*, arguments);
						TypeHolder result;

						result.int16 = Generic::Internal::c_system_internal::Instance().GetEffectShaderVariableIndexByName(args->effect_index,
																																							args->test_string);

						return result.pointer;
					}

					void *HS_SetEffectShaderVariableBoolean(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							ushort script_variable_index;
							ushort : 16;
							bool value;
							ushort : 16;
							byte   : 8;
							real interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableBoolean(
							args->effect_index, args->script_variable_index,
							args->value,
							args->interp_time);

						return NULL;
					}

					void *HS_SetEffectShaderVariableInteger(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							ushort script_variable_index;
							ushort : 16;
							uint32 value;
							real   interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableInteger(
							args->effect_index, args->script_variable_index,
							args->value,
							args->interp_time);

						return NULL;
					}

					void *HS_SetEffectShaderVariableReal(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							ushort script_variable_index;
							ushort : 16;
							real values[4];
							real interp_time;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableReal(
							args->effect_index, args->script_variable_index,
							args->values[0],
							args->values[1],
							args->values[2],
							args->values[3],
							args->interp_time);

						return NULL;
					}

					void *HS_SetEffectShaderInstanceActive(void **arguments) {
						struct s_arguments {
							ushort effect_index;
							ushort : 16;
							ushort shader_instance_index;
							ushort : 16;
							bool active;
						} *args = CAST_PTR(s_arguments*, arguments);

						Generic::Internal::c_system_internal::Instance().SetEffectShaderInstanceActive(
							args->effect_index,
							args->shader_instance_index,
							args->active);

						return NULL;
					}
				};
			};
		};
	};
};
