/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Model
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_MODEL
#include "Memory/_EngineLayout.inl"

	struct s_shader_feature_mix {
		const _enum		feature_mask;
		PAD16;
		const uint32	shader_index_offset;
		const uint32	shader_index_half_offset;
		const char		feature_mix_id[64];
	};

	struct s_vertex_shader_variables {
		real_vector2d	detail_normal_1_scale;
		real_vector2d	detail_normal_2_scale;
	};

	struct s_pixel_shader_variables {
		real base_normal_map_coefficient;
		real base_normal_map_z_coefficient;
		real c_spec_color_exponent_mask_toggle;
		PAD32;

		real detail_normal_map_1_interpolation;
		real detail_normal_map_2_interpolation;
		real detail_normal_map_1_coefficient;
		real detail_normal_map_2_coefficient;

		real specular_color_map_exponent;
		real specular_color_map_coefficient;
		real specular_lighting_exponent;
		real specular_lighting_coefficient;
	};

#pragma region Shader Features
	static const shader_usage_id_t g_base_shader_list[] =
	{
		"MaskDetailBeforeReflectionBiasedMultiply",
		"MaskDetailBeforeReflectionMultiply",
		"MaskDetailBeforeReflectionBiasedAdd",
		"MaskDetailAfterReflectionBiasedMultiply",
		"MaskDetailAfterReflectionMultiply",
		"MaskDetailAfterReflectionBiasedAdd",
		"MaskInverseDetailBeforeReflectionBiasedMultiply",
		"MaskInverseDetailBeforeReflectionMultiply",
		"MaskInverseDetailBeforeReflectionBiasedAdd",
		"MaskInverseDetailAfterReflectionBiasedMultiply",
		"MaskInverseDetailAfterReflectionMultiply",
		"MaskInverseDetailAfterReflectionBiasedAdd",

		"MaskDetailBeforeReflectionBiasedMultiplyComplexFog",
		"MaskDetailBeforeReflectionMultiplyComplexFog",
		"MaskDetailBeforeReflectionBiasedAddComplexFog",
		"MaskDetailAfterReflectionBiasedMultiplyComplexFog",
		"MaskDetailAfterReflectionMultiplyComplexFog",
		"MaskDetailAfterReflectionBiasedAddComplexFog",
		"MaskInverseDetailBeforeReflectionBiasedMultiplyComplexFog",
		"MaskInverseDetailBeforeReflectionMultiplyComplexFog",
		"MaskInverseDetailBeforeReflectionBiasedAddComplexFog",
		"MaskInverseDetailAfterReflectionBiasedMultiplyComplexFog",
		"MaskInverseDetailAfterReflectionMultiplyComplexFog",
		"MaskInverseDetailAfterReflectionBiasedAddComplexFog",
	};

	s_shader_feature_mix g_feature_mix_list[] = {
		{
			Flags::_shader_extension_usage_none,
			NUMBEROF(g_base_shader_list) * 0,
			(NUMBEROF(g_base_shader_list) / 2) * 0,
			""
		},
		{
			Flags::_shader_extension_usage_normal_map,
			NUMBEROF(g_base_shader_list) * 1,
			(NUMBEROF(g_base_shader_list) / 2) * 1,
			"BaseNorm"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 2,
			(NUMBEROF(g_base_shader_list) / 2) * 2,
			"BaseNormSpecMap"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 3,
			(NUMBEROF(g_base_shader_list) / 2) * 3,
			"BaseNormSpecLighting"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_specular_map | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 4,
			(NUMBEROF(g_base_shader_list) / 2) * 4,
			"BaseNormSpecMapSpecLighting"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_detail_normal,
			NUMBEROF(g_base_shader_list) * 5,
			(NUMBEROF(g_base_shader_list) / 2) * 5,
			"BaseNormDetailNorm"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 6,
			(NUMBEROF(g_base_shader_list) / 2) * 6,
			"BaseNormDetailNormSpecMap"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 7,
			(NUMBEROF(g_base_shader_list) / 2) * 7,
			"BaseNormDetailNormSpecLighting"
		},
		{
			Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_map | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 8,
			(NUMBEROF(g_base_shader_list) / 2) * 8,
			"BaseNormDetailNormSpecMapSpecLighting"
		},
		{
			Flags::_shader_extension_usage_detail_normal,
			NUMBEROF(g_base_shader_list) * 9,
			(NUMBEROF(g_base_shader_list) / 2) * 9,
			"DetailNorm"
		},
		{
			Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 10,
			(NUMBEROF(g_base_shader_list) / 2) * 10,
			"DetailNormSpecMap"
		},
		{
			Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 11,
			(NUMBEROF(g_base_shader_list) / 2) * 11,
			"DetailNormSpecLighting"
		},
		{
			Flags::_shader_extension_usage_detail_normal | Flags::_shader_extension_usage_specular_map | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 12,
			(NUMBEROF(g_base_shader_list) / 2) * 12,
			"DetailNormSpecMapSpecLighting"
		},
		{
			Flags::_shader_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 13,
			(NUMBEROF(g_base_shader_list) / 2) * 13,
			"SpecMap"
		},
		{
			Flags::_shader_extension_usage_specular_map | Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 14,
			(NUMBEROF(g_base_shader_list) / 2) * 14,
			"SpecMapSpecLighting"
		},
		{
			Flags::_shader_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 15,
			(NUMBEROF(g_base_shader_list) / 2) * 15,
			"SpecLighting"
		},
	};

	// pointer map to match feature usage mask to feature mix
	static s_shader_feature_mix* g_shader_feature_map[] =
	{
		&g_feature_mix_list[0],
		&g_feature_mix_list[1],
		&g_feature_mix_list[9],
		&g_feature_mix_list[5],
		&g_feature_mix_list[13],
		&g_feature_mix_list[2],
		&g_feature_mix_list[10],
		&g_feature_mix_list[6],
		&g_feature_mix_list[15],
		&g_feature_mix_list[3],
		&g_feature_mix_list[11],
		&g_feature_mix_list[7],
		&g_feature_mix_list[14],
		&g_feature_mix_list[4],
		&g_feature_mix_list[12],
		&g_feature_mix_list[8]
	};

	static shader_usage_id_t g_shader_usage_id_list[NUMBEROF(g_base_shader_list) * NUMBEROF(g_feature_mix_list)];

	static IDirect3DPixelShader9* g_environment_no_pixel_shaders[NUMBEROF(g_shader_usage_id_list) / 2];
	static IDirect3DPixelShader9* g_self_illumination_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_change_color_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_multipurpose_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_reflection_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_no_pixel_shaders[NUMBEROF(g_shader_usage_id_list) / 2];
#pragma endregion
	
	bool								g_rasterizer_model_normal_mapping = true;
	bool								g_rasterizer_model_detail_normal_mapping = true;
	bool								g_rasterizer_model_specular_lights = true;
	bool								g_rasterizer_model_specular_map = true;
	static s_shader_feature_mix*		g_current_feature_mix = &g_feature_mix_list[0];
	static s_vertex_shader_variables	g_vertex_shader_variables;
	static s_pixel_shader_variables		g_pixel_shader_variables;
	static _enum						g_extension_usage_mask;

#pragma region Hooks
	// hooks to correctly skip past inverse shaders in the usage id list
	__declspec(naked) static void Hook_EnvironmentNoPixelShaderIDOffset()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_RETN);

		_asm{
			// preserve ecx and edx
			push	ecx
			push	edx

			// move  the shader index to eax
			mov		eax, esi

			// divide eax by 6 to get the offset multiplier
			xor		edx, edx
			mov		ecx, 6
			div		ecx

			// multiply eax by 300h to get the shader id byte offset in ecx
			imul	ecx, eax, 300h
			lea		eax, [edi+ecx]

			pop		edx
			pop		ecx
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_NoPixelShaderIDOffset()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_RETN);

		_asm{
			// preserve ecx and edx
			push	ecx
			push	edx

			// move  the shader index to eax
			mov		eax, esi

			// divide eax by 6 to get the offset multiplier
			xor		edx, edx
			mov		ecx, 6
			div		ecx

			// multiply eax by 300h to get the shader id byte offset in ecx
			imul	ecx, eax, 300h
			lea		eax, [edi+ecx]

			pop		edx
			pop		ecx
			jmp		RETN_ADDRESS
		};
	}
	// hooks to return the new pixel shaders when applicable by offseting the shader index
	__declspec(naked) static void Hook_EnvironmentNoPixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		ecx, [ebx + 8]
no_extension:
			pop		ebx
			mov		edi, g_environment_no_pixel_shaders[ecx * 4]
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_EnvironmentNoInversePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		eax, [ebx + 8]
no_extension:
			pop		ebx
			mov		edi, g_environment_no_pixel_shaders[18h + eax * 4]
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_ChangeColorPixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_change_color_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_ChangeColorInversePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_change_color_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_MultipurposePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_multipurpose_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_MultipurposeInversePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_multipurpose_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_NoPixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_NO_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 8]
no_extension:
			pop		ebx
			mov		eax, g_no_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_ReflectionPixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_reflection_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_ReflectionInversePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_reflection_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	__declspec(naked) static void Hook_SelfIlluminationPixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_self_illumination_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_SelfIlluminationInversePixelShader()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_self_illumination_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	__declspec(naked) static void Hook_RenderObject_ForceInvertBackfaceNormals()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN);

		_asm{
			mov     al, 1
			test    al, al

			jmp		RETN_ADDRESS
		};
	}
#pragma endregion

	void SetModelNormSpec(void* shader_pointer)
	{
		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled)
			return;

		// reset to defaults
		g_current_feature_mix = nullptr;

		memset(&g_vertex_shader_variables, 0, sizeof(g_vertex_shader_variables));

		g_pixel_shader_variables.base_normal_map_coefficient = 1.0f;
		g_pixel_shader_variables.base_normal_map_z_coefficient = 1.0f;
		g_pixel_shader_variables.c_spec_color_exponent_mask_toggle = 0.0f;

		g_pixel_shader_variables.detail_normal_map_1_interpolation = 0.0f;
		g_pixel_shader_variables.detail_normal_map_2_interpolation = 0.0f;
		g_pixel_shader_variables.detail_normal_map_1_coefficient = 1.0f;
		g_pixel_shader_variables.detail_normal_map_2_coefficient = 1.0f;

		g_pixel_shader_variables.specular_lighting_exponent = 1.0f;
		g_pixel_shader_variables.specular_lighting_coefficient = 0.0f;
		g_pixel_shader_variables.specular_color_map_exponent = 1.0f;
		g_pixel_shader_variables.specular_color_map_coefficient = 1.0f;

		auto* shader_base = CAST_PTR(TagGroups::s_shader_definition*, shader_pointer);

		DX9::c_gbuffer_system::OutputObjectTBN() = false;

		// disable feature as per the users settings
		_enum feature_usage = shader_base->shader.extension_usage & g_extension_usage_mask;
		
		if(!g_rasterizer_model_normal_mapping)
		{
			feature_usage &= ~Flags::_shader_extension_usage_normal_map;
		}
		if(!g_rasterizer_model_detail_normal_mapping)
		{
			feature_usage &= ~Flags::_shader_extension_usage_detail_normal;
		}
		if(!g_rasterizer_model_specular_lights || !DebugOptions()->environment_specular_lights)
		{
			feature_usage &= ~Flags::_shader_extension_usage_specular_lighting;
		}
		if(!g_rasterizer_model_specular_map)
		{
			feature_usage &= ~Flags::_shader_extension_usage_specular_map;
		}

		g_current_feature_mix = g_shader_feature_map[feature_usage];

		if(shader_base->shader.shader_type == Enums::_shader_type_model)
		{
			auto* shader_model = CAST_PTR(TagGroups::s_shader_model_definition*, shader_base);

			if(shader_model->model.maps.shader_extension.Count == 1)
			{
				const auto& extension = shader_model->model.maps.shader_extension[0];

				// maps must have been built with build-cache-file-ex for the usage enum to be set

				// base normal map is present
				if(feature_usage & Flags::_shader_extension_usage_normal_map)
				{
					DX9::c_gbuffer_system::OutputObjectTBN() = true;
					SetTexture(Yelo::DX9::Direct3DDevice(), 4, extension.base_normal.map.tag_index);

					g_pixel_shader_variables.base_normal_map_coefficient = extension.base_normal.modifiers.coefficient;

					real z_multiplier = __max(1.0f / extension.base_normal.modifiers.coefficient, 0.0f);
					g_pixel_shader_variables.base_normal_map_z_coefficient = __min(z_multiplier, 1.0f);
				}

				// either of the detail normal maps are present
				if(feature_usage & Flags::_shader_extension_usage_detail_normal)
				{
					// if there is no base normal map then detail normal 1 is the only one present
					// however it might not be present if there is a base normal map so check whether its null
					if(!extension.detail_normals[0].map.tag_index.IsNull())
					{
						SetTexture(Yelo::DX9::Direct3DDevice(), 5, extension.detail_normals[0].map.tag_index);
						g_pixel_shader_variables.detail_normal_map_1_coefficient = extension.detail_normals[0].modifiers.coefficient;

						g_vertex_shader_variables.detail_normal_1_scale.Set(extension.detail_normals[0].modifiers.scale, 
							extension.detail_normals[0].modifiers.scale * extension.detail_normals[0].modifiers.v_scale);

						g_pixel_shader_variables.detail_normal_map_1_interpolation = 1.0f;
					}
					else
						g_pixel_shader_variables.detail_normal_map_1_interpolation = 0.0f;

					// if a base normal map is present either detail normal map may be present so check for the second map
					if(feature_usage & Flags::_shader_extension_usage_normal_map)
					{
						if(!extension.detail_normals[1].map.tag_index.IsNull())
						{
							SetTexture(Yelo::DX9::Direct3DDevice(), 6, extension.detail_normals[1].map.tag_index);
							g_pixel_shader_variables.detail_normal_map_2_coefficient = extension.detail_normals[1].modifiers.coefficient;

							g_vertex_shader_variables.detail_normal_2_scale.Set(extension.detail_normals[1].modifiers.scale,
								extension.detail_normals[1].modifiers.scale * extension.detail_normals[1].modifiers.v_scale);

							g_pixel_shader_variables.detail_normal_map_2_interpolation = 1.0f;
						}
						else
							g_pixel_shader_variables.detail_normal_map_2_interpolation = 0.0f;
					}
				}

				// setup specular map
				if(shader_model->shader.extension_usage & Flags::_shader_extension_usage_specular_map)
				{
					SetTexture(Yelo::DX9::Direct3DDevice(), 7, extension.specular_color.map.tag_index);

					g_pixel_shader_variables.specular_color_map_coefficient = extension.specular_color.modifiers.coefficient;
					g_pixel_shader_variables.specular_color_map_exponent = extension.specular_color.modifiers.exponent;
				}

				// setup specular lighting
				if(shader_model->shader.extension_usage & Flags::_shader_extension_usage_specular_lighting)
				{
					g_pixel_shader_variables.specular_lighting_exponent = extension.specular_lighting_exponent;
					g_pixel_shader_variables.specular_lighting_coefficient = extension.specular_lighting_coefficient;

					if(shader_model->shader.extension_usage & Flags::_shader_extension_usage_specular_map)
						if(extension.specular_color.modifiers.flags.alpha_as_exponent_mask_bit)
							g_pixel_shader_variables.c_spec_color_exponent_mask_toggle = 1.0f;

				}

				// copy the specular colour values to the main colour set to override them
				shader_model->model.reflection_properties.parallel_brightness = extension.parallel_brightness;
				shader_model->model.reflection_properties.parallel_tint_color = extension.parallel_tint_color;
				shader_model->model.reflection_properties.perpendicular_brightness = extension.perpendicular_brightness;
				shader_model->model.reflection_properties.perpendicular_tint_color = extension.perpendicular_tint_color;
			}
		}
		else if(shader_base->shader.shader_type == Enums::_shader_type_environment)
		{
			auto* shader_environment = CAST_PTR(TagGroups::s_shader_environment_definition*, shader_base);

			g_current_feature_mix = nullptr;

			// setup base normal map
			if(!shader_environment->environment.bump.bump_map.map.tag_index.IsNull())
			{
				g_current_feature_mix = &g_feature_mix_list[1];

				DX9::c_gbuffer_system::OutputObjectTBN() = true;

				SetTexture(Yelo::DX9::Direct3DDevice(), 4, shader_environment->environment.bump.bump_map.map.tag_index);

				g_pixel_shader_variables.base_normal_map_coefficient = shader_environment->environment.bump.bump_map.scale;

				real z_multiplier = __max(1.0f / shader_environment->environment.bump.bump_map.scale, 0.0f);
				g_pixel_shader_variables.base_normal_map_z_coefficient = __min(z_multiplier, 1.0f);
			}
			else
				Yelo::DX9::Direct3DDevice()->SetTexture(4, nullptr);
		}
		else
		{
			Yelo::DX9::Direct3DDevice()->SetTexture(4, nullptr);
			Yelo::DX9::Direct3DDevice()->SetTexture(5, nullptr);
			Yelo::DX9::Direct3DDevice()->SetTexture(6, nullptr);
			Yelo::DX9::Direct3DDevice()->SetTexture(7, nullptr);
		}
		Yelo::DX9::Direct3DDevice()->SetPixelShaderConstantF(0 + k_shader_constant_offset, (float*)&g_pixel_shader_variables, k_vector_4_count);
		Yelo::DX9::Direct3DDevice()->SetVertexShaderConstantF(15, (float*)&g_vertex_shader_variables, 1);
	}

	void		BuildShaderIDs()
	{
		for(int i = 0; i < NUMBEROF(g_feature_mix_list); i++)
		{
			for(int j = 0; j < NUMBEROF(g_base_shader_list); j++)
			{
				uint32 id_index = (i * NUMBEROF(g_base_shader_list)) + j;
				strcat_s(g_shader_usage_id_list[id_index], 128, g_base_shader_list[j]);
				strcat_s(g_shader_usage_id_list[id_index], 128, g_feature_mix_list[i].feature_mix_id);
			}
		}
	}

	void		ApplyHooks()
	{
		BuildShaderIDs();

		// change the shader usage id offsets on effects that only have 12 shaders
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoPixelShaderIDOffset,	GET_FUNC_VPTR(RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_NoPixelShaderIDOffset,				GET_FUNC_VPTR(RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_HOOK), true);
		
		// change shader usage id pointers to the extended array
		for (auto ptr : K_SHADER_USAGE_ID_ARRAY_REFERENCES)
			*ptr = CAST_PTR(void*, &g_shader_usage_id_list[0][0]);

		// update with the extended shader count
		GET_PTR(RASTERIZER_MODEL_SHADER_LOAD_COUNT) = NUMBEROF(g_shader_usage_id_list);
		GET_PTR(RASTERIZER_MODEL_SHADER_LOAD_NO_INVERSE_COUNT) = NUMBEROF(g_shader_usage_id_list) / 2;

		// redirect references to the original pixel shader arrays to the custom arrays
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO)		*ptr = &g_environment_no_pixel_shaders;
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR)			*ptr = &g_change_color_pixel_shaders;
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE)			*ptr = &g_multipurpose_pixel_shaders;
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_NO)					*ptr = &g_no_pixel_shaders;
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_REFLECTION)			*ptr = &g_reflection_pixel_shaders;
		for (auto ptr : K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION)	*ptr = &g_self_illumination_pixel_shaders;

		// change the pixel shader index to return the normal map shaders when appropriate
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ChangeColorPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ChangeColorInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_MultipurposePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_MultipurposeInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_NoPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ReflectionPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ReflectionInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_SelfIlluminationPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_SelfIlluminationInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_HOOK), true);
	}

	bool		SetViewProj(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount)
	{
		if((g_ps_support > _ps_support_2_0) && g_extensions_enabled)
		{
			device->SetPixelShaderConstantF(4 + k_shader_constant_offset,
				&pConstantData[4 * 4],
				1);
		}
		return true;
	}

	bool		SetSpecColor(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount)
	{
		if((g_ps_support > _ps_support_2_0) && g_extensions_enabled)
		{
			device->SetPixelShaderConstantF(5 + k_shader_constant_offset,
				pConstantData,
				2);
		}
		return true;
	}

	bool		SetVertexLight(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount)
	{
		if((g_ps_support > _ps_support_2_0) && g_extensions_enabled)
		{
			device->SetPixelShaderConstantF(7 + k_shader_constant_offset,
				pConstantData,
				Vector4fCount);
		}
		return true;
	}

	void SetNormalMapsEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_normal_map;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_normal_map : 0);
	}

	bool GetNormalMapsEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_normal_map);
	}

	void SetDetailNormalMapsEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_detail_normal;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_detail_normal : 0);
	}

	bool GetDetailNormalMapsEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_detail_normal);
	}

	void SetSpecularMapsEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_specular_map;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_specular_map : 0);
	}

	bool GetSpecularMapsEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_specular_map);
	}

	void SetSpecularLightingEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_specular_lighting;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_specular_lighting : 0);
	}

	bool GetSpecularLightingEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_specular_lighting);
	}

};
