/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static TagGroups::s_pass_definition			g_shader_passes_sm2[] = 
{
	{ "BrightPass",	{0, 0, 0}, 0 },
	{ "BlurPass1",	{0, 0, 1}, 1 },
	{ "BlurPass2",	{0, 0, 0}, 1 },
	{ "Composite",	{0, 0, 0}, 0 },
};
static TagGroups::s_pass_definition			g_shader_passes_sm3[] = 
{
	{ "BrightPass",	{0, 0, 0}, 0 },
	{ "BlurPass1",	{0, 0, 1}, 0 },
	{ "BlurPass2",	{0, 0, 0}, 0 },
	{ "Composite",	{0, 0, 0}, 0 },
};
static TagGroups::s_technique_definition	g_shader_techniques[] =
{
	// empty technique since bloom shouldn't be available on shader model 1.0
	{
		"",
		{ 1, 0, 0 },
		{ 0 },
		{ 0, NULL, NULL }
	},
	// technique for shader model 2.0 which uses gaussian blur so that the shader is not as complex
	{
		"PostProcess_sm2_0",
		{ 0, 1, 0 },
		{ 0 },
		{ NUMBEROF(g_shader_passes_sm2), &g_shader_passes_sm2[0], NULL }
	},
	// technique for shader model 3.0 which uses poisson blur so that the bloom is nice and smooth
	{
		"PostProcess_sm3_0",
		{ 0, 0, 1 },
		{ 0 },
		{ NUMBEROF(g_shader_passes_sm3), &g_shader_passes_sm3[0], NULL }
	}
};

static TagGroups::s_shader_postprocess_globals_bloom
											g_bloom_defaults =
{
	{ 1, 1 },
	{ 3.5f, 1.0f, 0.5f, { 0.55f, 0.55f, 0.55f }, { 1.0f, 1.0f, 1.0f } }
};
static s_shader_bloom_definition						g_shader_bloom_definition;
static TagGroups::s_effect_postprocess_definition		g_effect_bloom_definition;