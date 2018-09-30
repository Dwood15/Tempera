#pragma once

/*
struct s_render_camera
{
	real_point3d point;
	real_vector3d forward;
	real_vector3d up;

	UNKNOWN_TYPE(bool); // controls the rendering of stuff related to certain objects
	PAD24;
	real vertical_field_of_view;
	rectangle2d viewport_bounds;
	point2d window_bounds;
	real z_near, z_far;
	int __PAD32[4]; // just looks like 16 bytes of unused poop
}; BOOST_STATIC_ASSERT(sizeof(s_render_camera) == 0x54)

struct s_render_window
{
	int16 local_player_index;
	UNKNOWN_TYPE(bool);
	PAD8;
	s_render_camera render_camera, rasterizer_camera;
}; static_assert(sizeof(s_render_window) == 0xAC);

struct s_render_fog
{
	word_flags flags; // same flags as in the tag definition
	short : 16;
	real_rgb_color atmospheric_color;
	real atmospheric_maximum_density,
		atmospheric_minimum_distance,
		atmospheric_maximum_distance;
	UNKNOWN_TYPE(int16);
	short : 16;
	real_plane3d plane; // copied from the bsp's fog planes
	real_rgb_color color; // copied from tag definition
	real max_density; // copied from tag definition
	real planar_maximum_distance;
	real opaque_depth; // copied from tag definition
	const void* screen_layers_definition; // pointer to the tag definition data
	UNKNOWN_TYPE(real);
}; static_assert(sizeof(s_render_fog) == 0x50);

struct s_render_globals
{
	int		__iUnk0;
	int		__iUnk1;
	short local_player_index;
	short __sUnk0;
	long __lUnk0;
	real __rUnk0;
	byte camera[];
	s_render_frustum frustum;
	s_render_fog fog;
	int leaf_index;
	int cluster_index;
	char pad; // probably an unused bool
	bool visible_sky_model;
	short visible_sky_index;

	struct {
		long cluster_pvs[512];
		byte clusters[0x1A0][k_maximum_rendered_clusters];
		short count;
		short pad;
	}rendered_clusters;

	struct {
		unsigned int visibility_bitvector[0x20000];
		int count;
		unsigned int triangles[k_maximum_rendered_triangles];
	}rendered_triangles;
}; static_assert(sizeof(s_render_globals) == 0x9D298);
//s_render_globals* RenderGlobals(); // defined in the implementing extension's code
*/

#include <macros_generic.h>
#include "../math/real_math.h"

struct s_structure_render_globals {
	bool          render_bsp;
	char          pad[3];                        //0x1
	int           dynamic_triangle_buffer_index;  //0x4
	char          __UNKNOWN_TYPE;                //0x8
	char          pad1[3]; //0x9
	real_vector3d __guessed_type; //0xC
	//0x18
};
STAT_ASSERT(s_structure_render_globals, 0x18);
//s_structure_render_globals* StructureRenderGlobals(); // defined in the implementing extension's code


namespace render{
	int main_get_window_count_override();

	/*
	 * 0x4CCF76
	 * - E8 75 EF FF FF
	 *
	 * 0x53BF45
	 * - E8 A6 FF F8 FF
	 *
	 * 0x53D018
	 * - E8 D3 EE F8 FF
	 *
	 * 0x53D0B8
	 * - E8 33 EE F8 FF
	 */
	int main_get_current_solo_level(char * level_string);

	/**
	 * 0x49757D - interface_draw_screen
	 * - E8 FE 4F 03 00
	 *network
	 * 0x49792B - see above
    * - E8 50 4C 03 00
	 *
	 * 0x4975C4 - check_render_split_screen
	 * - E8 B7 4F 03 00
	 *
	 * 0x51EB00 rasterizer_detail_objects_begin
	 * - E8 7B DA FA FF
	 *
	 * 0x51EE00 - rasterizer_detail_objects_rebuild_vertices
	 * - E8 7B D7 FA FF
	 *
	 * 0x51EFA3 - rasterizer_detail_objects_draw
	 * - E8 D8 D5 FA FF
	 */
	namespace overrides {
		void override_all();
	};

};
