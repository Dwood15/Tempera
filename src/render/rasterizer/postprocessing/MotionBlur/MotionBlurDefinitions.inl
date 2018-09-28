/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static TagGroups::s_pass_definition			g_shader_passes_sm3[] = 
{
	{
		"Mask",
		{0, 0, 0},
		0
	},
	{
		"Blur",
		{0, 0, 0},
		0
	}
};
static TagGroups::s_technique_definition	g_shader_techniques[] =
{
	{
		"",
		{ 1, 1, 0 },
		{ 0 },
		{ 0, NULL, NULL }
	},
	{
		"PostProcess", 
		{ 0, 0, 1 },
		{ 0 }, 
		{ NUMBEROF(g_shader_passes_sm3), &g_shader_passes_sm3[0], NULL }
	}
};
static s_shader_motionblur_definition					g_shader_motionblur_definition;
static TagGroups::s_effect_postprocess_definition		g_effect_motionblur_definition;