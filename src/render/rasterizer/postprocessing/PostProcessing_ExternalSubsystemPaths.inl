/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
// TODO: why are these not relative?
#if		defined(SUBSYSTEM_BLOOM)
	static const char* g_external_subsystem_shader_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\PP_Bloom.fx";
	static const char* g_external_subsystem_include_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\";
#elif	defined(SUBSYSTEM_MOTIONBLUR)
	static const char* g_external_subsystem_shader_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\PP_MotionBlur.fx";
	static const char* g_external_subsystem_include_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\";
#elif	defined(SUBSYSTEM_FADE)
	static const char* g_external_subsystem_shader_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\PP_EffectFade.fx";
	static const char* g_external_subsystem_include_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\";
#elif	defined(SUBSYSTEM_FXAA)
	static const char* g_external_subsystem_shader_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\PP_FXAA.fx";
	static const char* g_external_subsystem_include_path = "D:\\Program Files (x86)\\Microsoft Games\\Halo Custom Edition\\data\\postprocess\\dev_shaders\\";
#endif