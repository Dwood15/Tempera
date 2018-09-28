/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static TagGroups::s_pass_definition			g_shader_passes[1] = 
{
	{ "EffectFade", { 0, 0 }, 0 },
};
static TagGroups::s_technique_definition	g_shader_techniques[1] =
{
	{
		"PostProcess",
		{ 1, 1, 1 },
		{ 0 },
		{ NUMBEROF(g_shader_passes), &g_shader_passes[0], NULL }
	}
};

static s_shader_fade_definition			g_shader_fade_definition;