#pragma once
/**
 * <GPLv3 License>
 */
#include "../ce_base_types.h"
#include "int_math.h"
#include "real_math.h"

#pragma pack(push)
#pragma pack(1)
namespace obj {
	namespace sbsp {
		struct light_color_direction {
			real_rgb_color color;
			real_vector3d  color_direction;
		};
		namespace coll {
			struct bsp3d_node {
				long plane;
				long back_child;
				long front_child;
			}; STAT_ASSRT(bsp3d_node, 0xC);

			struct collision_leaf {
				short flags;
				short reference_count;
				long  first_reference;
			}; STAT_ASSRT(collision_leaf, 0x8);

			struct bsp2d_reference {
				long plane;
				long bsp2d_node;
			}; STAT_ASSRT(bsp2d_reference, 0x8);

			struct bsp2d_node {
				real_plane2d plane;
				long         left_child;
				long         right_child;
			}; STAT_ASSRT(bsp2d_node, 0x14);

			struct collision_surface {
				long                      plane;
				long                      first_edge;
				::collision_surface_flags flags;
				sbyte                     breakable_surface;
				short                     material;
			}; STAT_ASSRT(collision_surface, 0xC);

			struct collision_edge {
				long start_vertex;
				long end_vertex;
				long forward_edge;
				long reverse_edge;
				long left_surface;
				long right_surface;
			}; STAT_ASSRT(collision_edge, 0x18);

			struct collision_vertex {
				real_point3d point;
				long         first_edge;
			}; STAT_ASSRT(collision_vertex, 0x10);

			struct collision_bsp {
				tag_block<bsp3d_node>     bsp3d_nodes;
				tag_block<real_plane3d>   planes;
				tag_block<collision_leaf> leaves;

				//bsp2d block(s).
				tag_block<bsp2d_reference> references;
				tag_block<bsp2d_node>      nodes;

				tag_block<collision_surface> surfaces;
				tag_block<collision_edge>    edges;
				tag_block<collision_vertex>  vertices;
			}; STAT_ASSRT(collision_bsp, 0x60);
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
		}; STAT_ASSRT(s_object_lighting, 0x74); //, "s_object_lighting stat assert fail");

		struct structure_collision_material {
			tag_reference shader; // 'shdr'
			short         runtime_pad16;
			short         runtime_shader_material_type; // Enums::material_type or NONE if shdr is null
		}; STAT_ASSRT(structure_collision_material, 0x14);

		struct structure_node {
			sbsp_stri unk_node_info;
		};

		struct structure_leaf {
			byte  skip[6];
			short pad16;
			short cluster;
			short reference_count; //"surface"
			//TODO: check if datum_index or pointer.
			long  first_reference; //"surface"
		};

		struct structure_surface_reference {
			long surface; //TODO: Is this a datum index or regular pointer?
			long index;
		};

		struct sbsp_tag {
			static inline int                                  group_tag = (int) 'sbsp';
			tag_reference                                      lightmap_bitm;
			real_bounds                                        vehicle_heights; //floor, ceil
			int32                                              __pad00[5];
			obj::sbsp::s_object_lighting                       default_lighting;
			int32                                              __pad01;
			tag_block<obj::sbsp::structure_collision_material> collision_materials;
			//TODO: Structure bsp tag block ! tag_block<obj::sbsp
		};

	};
};

#pragma pack(pop)