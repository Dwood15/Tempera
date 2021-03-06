#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_DPTR(type, name, ...) static auto** const name = reinterpret_cast<type**>(__VA_ARGS__);
#define ENGINE_PTR(type, name, ...) static auto* const name = reinterpret_cast<type*>(__VA_ARGS__);
#define DUO_PTR(...) __VA_ARGS__
#define FUNC_PTR(name, value, ...) static auto const name  =  value
#define CAST_PTR(type, ptr)      reinterpret_cast<type>(ptr)

//////////////////////////////////////////////////////////////////////////
// ShaderExtension.cpp
static auto const RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK = K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK;
static auto const RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN = K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN;

static auto const RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL = K_RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL;

	static const char ** K_VSH_COLLECTION_PATH_REFERENCES[] = {
		reinterpret_cast<const char * *>(K_VSH_COLLECTION_PATH_REFERENCE_0),
		reinterpret_cast<const char * *>(K_VSH_COLLECTION_PATH_REFERENCE_1),
	};

	static void** K_PS_VERSION_ITERATOR_START[] = {
		reinterpret_cast<void **>(K_PS_VERSION_ITERATOR_START_0),
		reinterpret_cast<void **>(K_PS_VERSION_ITERATOR_START_1),
	};

	static void** K_SHADER_USAGE_ID_ARRAY_REFERENCES[] = {
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_0),
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_1),
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_2),
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_3),
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_4),
		reinterpret_cast<void **>(K_SHADER_USAGE_ID_ARRAY_REFERENCE_5),
	};

static auto *const RASTERIZER_MODEL_SHADER_LOAD_COUNT = reinterpret_cast<int *>(K_RASTERIZER_MODEL_SHADER_LOAD_COUNT);;
static auto *const RASTERIZER_MODEL_SHADER_LOAD_NO_INVERSE_COUNT = reinterpret_cast<int *>(K_RASTERIZER_MODEL_SHADER_LOAD_NO_INVERSE_COUNT);;

static auto const RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_HOOK = K_RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_HOOK;
static auto const RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_RETN = K_RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_RETN;

static auto const RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_HOOK = K_RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_HOOK;
static auto const RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_RETN = K_RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_RETN;


	static void** K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO_1),
	};
	static void** K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR_1),
	};
	static void** K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE_1),
	};
	static void** K_PIXEL_SHADER_REFERENCES_NO[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_NO_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_NO_1),
	};
	static void** K_PIXEL_SHADER_REFERENCES_REFLECTION[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_REFLECTION_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_REFLECTION_1),
	};
	static void** K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION[] = {
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION_0),
		reinterpret_cast<void **>(K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION_1),
	};

static auto const RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_HOOK = K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_RETN = K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_RETN;
static auto const RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_HOOK = K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_RETN = K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_RETN;

static auto const RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_HOOK = K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_RETN = K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_RETN;
static auto const RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_HOOK = K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_RETN = K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_RETN;

static auto const RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_HOOK = K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_RETN = K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_RETN;
static auto const RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_HOOK = K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_RETN = K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_RETN;

static auto const RASTERIZER_MODEL_PS_INDEX_NO_HOOK = K_RASTERIZER_MODEL_PS_INDEX_NO_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_NO_RETN = K_RASTERIZER_MODEL_PS_INDEX_NO_RETN;

static auto const RASTERIZER_MODEL_PS_INDEX_REFLECTION_HOOK = K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_REFLECTION_RETN = K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_RETN;
static auto const RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_HOOK = K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_RETN = K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_RETN;

static auto const RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_HOOK = K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_RETN = K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_RETN;
static auto const RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_HOOK = K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_HOOK;
static auto const RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_RETN = K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_RETN;

static auto const RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK = K_RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK;
static auto const RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN = K_RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN;

static auto const STRUCTURE_RENDER_DYNAMIC_TRIANGLES_COUNT_REF = K_STRUCTURE_RENDER_DYNAMIC_TRIANGLES_COUNT_REF;
static auto const STRUCTURE_RENDER_DYNAMIC_TRIANGLES_BUFFER_REF = K_STRUCTURE_RENDER_DYNAMIC_TRIANGLES_BUFFER_REF;
static auto const STRUCTURE_RENDER_PASS_FUNC = K_STRUCTURE_RENDER_PASS_FUNC;
static auto const LIGHTS_RENDER_SPECULAR_CALL = K_LIGHTS_RENDER_SPECULAR_CALL;
static auto const LIGHTS_RENDER_SPECULAR_FUNC = K_LIGHTS_RENDER_SPECULAR_FUNC;
static auto const RASTERIZER_ENVIRONMENT_LIGHTMAP_BEGIN_FUNC = K_RASTERIZER_ENVIRONMENT_LIGHTMAP_BEGIN_FUNC;
static auto const RASTERIZER_ENVIRONMENT_LIGHTMAP_DRAW_FUNC = K_RASTERIZER_ENVIRONMENT_LIGHTMAP_DRAW_FUNC;
static auto const RASTERIZER_ENVIRONMENT_LIGHTMAP_END_FUNC = K_RASTERIZER_ENVIRONMENT_LIGHTMAP_END_FUNC;

static auto const RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK = K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK;
static auto const RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN = K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN;
static auto const RASTERIZER_EFFECT_PARTICLE_HOOK = K_RASTERIZER_EFFECT_PARTICLE_HOOK;
static auto const RASTERIZER_EFFECT_PARTICLE_RETN = K_RASTERIZER_EFFECT_PARTICLE_RETN;
static auto const RASTERIZER_EFFECT_CONTRAIL_HOOK = K_RASTERIZER_EFFECT_CONTRAIL_HOOK;
static auto const RASTERIZER_EFFECT_CONTRAIL_RETN = K_RASTERIZER_EFFECT_CONTRAIL_RETN;

static auto const RASTERIZER_EFFECT_PS_INDEX_NO_HOOK = K_RASTERIZER_EFFECT_PS_INDEX_NO_HOOK;
static auto const RASTERIZER_EFFECT_PS_INDEX_NO_RETN = K_RASTERIZER_EFFECT_PS_INDEX_NO_RETN;
