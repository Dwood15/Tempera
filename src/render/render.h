#pragma once

#include <macros_generic.h>
#include <enums/geometry_enums.h>
#include "../math/colors.h"

struct s_render_camera
{
	real_point3d point;
	real_vector3d forward;
	real_vector3d up;

	char unkType; // controls the rendering of stuff related to certain objects
	char unkPad0;
	short unkPad1;
	real vertical_field_of_view;
	rectangle2d viewport_bounds;
	rectangle2d window_bounds;
	real z_near, z_far;
	int __PAD32[4]; // just looks like 16 bytes of unused poop
};
STAT_ASSERT(s_render_camera, 0x54);

#pragma pack(push, 1)
struct s_render_frustum
{
	real unkRealPad[4]; // 4 reals
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;

	// clipping bounds
	real_plane3d padPlane3d0;
	real_plane3d padPlane3d01;
	real_plane3d padPlane3d02;
	real_plane3d padPlane3d03;

	real_plane3d padPlane3d04;
	real_plane3d padPlane3d05;

	real z_near, z_far;

	real_point3d padPlane3d06;
	real_point3d padPlane3d07;
	real_point3d padPlane3d08;
	real_point3d padPlane3d09;
	real_point3d padPlane3d0A;
	real_point3d padPlane3d0B;
	real_point3d padPlane3d0C;
	real_point3d padPlane3d0D;

	bool projection_valid;
	unsigned char pad0;
	unsigned short pad1;

	// projection matrix
	unsigned __int64 pad2;
	unsigned __int64 pad3;
	unsigned __int64 pad4;
	unsigned __int64 pad5;
	unsigned __int64 pad6;
	unsigned __int64 pad7;
	unsigned __int64 pad8;
	unsigned __int64 pad9;

	real_point2d padA;
};
#pragma pack(pop)
STAT_ASSERT(s_render_frustum, 0x18C);

struct s_render_window
{
	int16 local_player_index;
	bool unkTypePad;
	char pad0;
	s_render_camera render_camera, rasterizer_camera;
};
STAT_ASSERT(s_render_window, 0xAC);

struct s_render_fog
{
	ushort flags; // same flags as in the tag definition
	ushort unkPad0;
	real_rgb_color atmospheric_color;
	real atmospheric_maximum_density,
		atmospheric_minimum_distance,
		atmospheric_maximum_distance;
	ushort unkPad1;
	ushort unkPad2;
	real_plane3d plane; // copied from the bsp's fog planes
	real_rgb_color color; // copied from tag definition
	real max_density; // copied from tag definition
	real planar_maximum_distance;
	real opaque_depth; // copied from tag definition
	const void* screen_layers_definition; // pointer to the tag definition data
	real unkTypePad;
};
STAT_ASSERT(s_render_fog,  0x50);

struct s_byteClusters {
	byte clusters[0x1A0][0x80];
};
STAT_ASSERT(s_byteClusters, 0xD000);

//STAT_ASSERT(s_renderedClusterPvs, 0x40);
struct s_renderedClustersSum {
	long cluster_pvs[16]; // (512 + 31) >> 5 )
	s_byteClusters clusters;
	short count;
	short pad;
};
STAT_ASSERT(s_renderedClustersSum, 0xD044);

struct s_render_globals
{
	int		__iUnk0; //0x0
	int		__iUnk1; //0x4
	short local_player_index; //0x8
	short __sUnk0; //0xA
	long __lUnk0; //0xC
	real __rUnk0; //0x10
	s_render_camera camera; //0x64
	s_render_frustum frustum; //0x1F0
	s_render_fog fog; //0x240
	int leaf_index;
	int cluster_index;
	char pad; // probably an unused bool
	bool visible_sky_model;
	short visible_sky_index;

	s_renderedClustersSum rendered_clusters;

	struct {
		unsigned int visibility_bitvector[0x20000];
		int count;
		unsigned int triangles[::Yelo::Enums::k_maximum_rendered_triangles];
	} rendered_triangles;
};
STAT_ASSERT(s_render_globals, 0x9D298);
//s_render_globals* RenderGlobals(); // defined in the implementing extension's code

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
