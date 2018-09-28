/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static TagGroups::s_pass_definition			g_shader_passes_sm3[] = 
{
	{ "Luma",	{0, 0, 0}, 0 },
	{ "FXAA",	{0, 0, 0}, 0 },
};
static TagGroups::s_technique_definition	g_shader_techniques[] =
{
	// empty technique since fxaa shouldn't be available on shader model 1.0 or 2.0
	{
		"",
		{ 1, 1, 0 },
		{ 0 },
		{ 0, NULL, NULL }
	},
	// technique for shader model 3.0
	{
		"PostProcess_sm3_0",
		{ 0, 0, 1 },
		{ 0 },
		{ NUMBEROF(g_shader_passes_sm3), &g_shader_passes_sm3[0], NULL }
	}
};

static TagGroups::s_shader_postprocess_definition			g_shader_fxaa_definition;
static TagGroups::s_effect_postprocess_definition			g_effect_fxaa_definition;