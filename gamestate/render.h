/**
 *	Project: Tempera
 *	File: render.h
 *	Copyright � 2018 Dwood
 *
 * This file is part of Tempera.
 *
 * Tempera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tempera is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once

#include "../headers/real_math.h"

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
	PAD16;
	real_rgb_color atmospheric_color;
	real atmospheric_maximum_density,
		atmospheric_minimum_distance,
		atmospheric_maximum_distance;
	UNKNOWN_TYPE(int16);
	PAD16;
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

struct s_structure_render_globals {
	bool          render_bsp;
	char          pad[3];                        //0x4
	int           dynamic_triangle_buffer_index;  //0x1C
	char          __UNKNOWN_TYPE;                //0x20
	char          pad1[3];
	real_vector3d __guessed_type;
}; //static_assert(sizeof(s_structure_render_globals) == 0x18);
//s_structure_render_globals* StructureRenderGlobals(); // defined in the implementing extension's code
