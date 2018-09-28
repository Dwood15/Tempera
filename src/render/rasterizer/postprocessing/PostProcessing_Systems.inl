/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static s_component_toggle<IPostProcessingComponent> g_postprocess_subsystems[] = 
{
	{ false, &Fade::c_system_fade::Instance() },
	{ false, &MotionBlur::c_system_motionblur::Instance() },
	{ false, &Bloom::c_system_bloom::Instance() },
	{ false, &FXAA::c_system_fxaa::Instance() },
	{ false, &Generic::Internal::c_system_internal::Instance() },
	{ false, &Generic::External::c_system_external::Instance() }
};

static s_component_toggle_ref<IPostProcessingCacheComponent> g_postprocess_cache_subsystems[] = 
{
	{ 2, &Bloom::c_system_bloom::Instance() },
	{ 4, &Generic::Internal::c_system_internal::Instance() }
};

static s_component_toggle_ref<IPostProcessingUpdatable> g_postprocess_updatable_subsystems[] = 
{
	{ 1, &MotionBlur::c_system_motionblur::Instance() },
	{ 2, &Bloom::c_system_bloom::Instance() },
	{ 4, &Generic::Internal::c_system_internal::Instance() },
	{ 5, &Generic::External::c_system_external::Instance() }
};

static s_component_toggle_ref<IPostProcessingRenderable> g_postprocess_renderable_subsystems[] = 
{
	{ 1, &MotionBlur::c_system_motionblur::Instance() },
	{ 2, &Bloom::c_system_bloom::Instance() },
	{ 3, &FXAA::c_system_fxaa::Instance() },
	{ 4, &Generic::Internal::c_system_internal::Instance() },
	{ 5, &Generic::External::c_system_external::Instance() }
};