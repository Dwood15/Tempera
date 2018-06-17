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
// ShaderDraw.cpp
static auto *const RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP_SHADER_DRAW__DRAW_SHADER_LIGHTMAP_PTR = reinterpret_cast<void **>(K_RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP_SHADER_DRAW__DRAW_SHADER_LIGHTMAP_PTR);;
static auto *const RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_ENVIRONMENT_PTR = reinterpret_cast<void **>(K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_ENVIRONMENT_PTR);;
static auto *const RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_MODEL_PTR = reinterpret_cast<void **>(K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_MODEL_PTR);;

static auto const RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP = K_RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP;
static auto const RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_ENVIRONMENT = K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_ENVIRONMENT;
static auto const RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL = K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL;
