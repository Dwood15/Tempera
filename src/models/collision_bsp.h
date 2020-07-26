#pragma once

#include <macros_generic.h>
#include <enums/generic_enums.h>
#include "../memory/datum_index.h"
#include "../math/colors.h"
#include "../math/real_math.h"
#include "../game/objects/object_lights.h"
#include "../tags/group/tagreference.h"

namespace Yelo::TagGroups {
	struct light_color_direction {
		real_rgb_color color;
		real_vector3d  color_direction;
	};

	//I'll admit this is less than ideal, however, I do believe this is the more sensible way of managing static asserts in simpler structures.
	//We're testing for issues in the current struct, hopefully the child structures will be guaranteed correct by a similar assert.
	STAT_ASSERT(light_color_direction, sizeof(real_rgb_color) + sizeof(real_vector3d));

	namespace coll {
		struct bsp3d_node {
			long plane;
			long back_child;
			long front_child;
		};
		STAT_ASSERT(bsp3d_node, 0xC);

		struct collision_leaf {
			short flags;
			short reference_count;
			long  first_reference;
		};
		STAT_ASSERT(collision_leaf, 0x8);

		struct bsp2d_reference {
			long plane;
			long bsp2d_node;
		};
		STAT_ASSERT(bsp2d_reference, 0x8);

		struct bsp2d_node {
			real_plane2d plane;
			long         left_child;
			long         right_child;
		};
		STAT_ASSERT(bsp2d_node, 0x14);

		struct collision_surface {
			long                      plane;
			long                      first_edge;
			::collision_surface_flags flags;
			sbyte                     breakable_surface;
			short                     material;
		};
		STAT_ASSERT(collision_surface, 0xC);

		struct collision_edge {
			long start_vertex;
			long end_vertex;
			long forward_edge;
			long reverse_edge;
			long left_surface;
			long right_surface;
		};
		STAT_ASSERT(collision_edge, 0x18);

		struct collision_vertex {
			real_point3d point;
			long         first_edge;
		};
		STAT_ASSERT(collision_vertex, sizeof(real_point3d) + 0x4);

		struct collision_bsp {
			TagBlock<bsp3d_node>     bsp3d_nodes;
			TagBlock<real_plane3d>   planes;
			TagBlock<collision_leaf> leaves;

			//bsp2d block(s).
			TagBlock<bsp2d_reference> references;
			TagBlock<bsp2d_node>      nodes;

			TagBlock<collision_surface> surfaces;
			TagBlock<collision_edge>    edges;
			TagBlock<collision_vertex>  vertices;
		};
		STAT_ASSERT(collision_bsp, 0x60);
	};

	struct s_object_lighting {
		real_rgb_color        ambient_color;
		short                 distant_light_count;
		short                 pad;
		light_color_direction distant_light[2];

		//////////////////////////////////////////////////////////////////////////
		// not tag based - used by the game state
		short       point_light_count;
		short       pad00;
		datum_index point_light_indices[2]; // light_data
		//////////////////////////////////////////////////////////////////////////

		real_argb_color reflection_tint;
		real_vector3d   shadow_vector;
		real_rgb_color  shadow_color;
	};
	STAT_ASSERT(s_object_lighting, 0x74); //, "s_object_lighting stat assert fail");

	struct structure_collision_material {
		tag_reference shader; // 'shdr'
		short         runtime_pad16;
		short         runtime_shader_material_type; // Enums::material_type or NONE if shdr is null
	};
	STAT_ASSERT(structure_collision_material, 0x14);

	struct structure_node {
		sbsp_stri unk_node_info;
	};
	STAT_ASSERT(structure_node, 0x6);


	struct structure_leaf {
		byte  skip[6];
		short pad16;
		short cluster;
		short reference_count; //"surface"
		//TODO: check if datum_index or pointer.
		long  first_reference; //"surface"
	};
	STAT_ASSERT(structure_leaf, 0x8 + 0x4 + 0x4);

	struct structure_surface_reference {
		long surface; //TODO: Is this a datum index or regular pointer?
		long index;
	};

	STAT_ASSERT(structure_surface_reference, 0x8);

	struct sbsp_tag {
		//Used to be static inline int. This is probably fucking up the sizeof on the tag tbqh.
		const int                       group_tag = (int) 'sbsp';
		tag_reference                           lightmap_bitm;
		real_bounds                             vehicle_heights; //floor, ceil
		int32                                   __pad00[5];
		Yelo::Objects::s_object_lighting        default_lighting;
		int32                                   __pad01;
		TagBlock<structure_collision_material> collision_materials;
		//TODO: Structure bsp tag block ! TagBlock<obj::sbsp
	};

	// STAT_ASSERT(structure_leaf, 0x8 + 0x4 + 0x4);

};
