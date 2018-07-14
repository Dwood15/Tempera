#pragma once

#include "../../render/rasterizer/geometry.h"
#include "../../tags/group/base.h"
#include "../../game/objects/object_lights.hpp"
#include "../../cache/predicted_resources.hpp"

namespace Yelo::TagGroups {
	using namespace Yelo;
	struct structure_collision_material {
		Yelo::tag_reference shader;
		struct {
			unsigned short : 16;
			short shader_material_type; // Enums::material_type or NONE if shdr is null
		}                   runtime;
	};

	struct structure_node {
		short skip[3];
	};
	struct structure_leaf {
		byte skip[6];
		unsigned short : 16;
		short cluster;
		short reference_count;
		long  first_reference;
	};
	struct structure_surface_reference {
		long surface;
		long index;
	};
	struct structure_surface {
		short a[3];
	};
	struct structure_bsp_material {
		tag_reference              shader;
		short                      shader_permutation;
		unsigned short             flags;
		long                       surfaces;
		long                       surface_count;
		real_point3d               centroid;
		Objects::s_object_lighting object_lighting;
		real_plane3d               plane;
		short                      breakable_surface;
		unsigned short : 16;

		Rasterizer::rasterizer_vertex_buffer
					vertices, lightmap_vertices;
		tag_data uncompressed_vertices;
		tag_data compressed_vertices;
	}; static_assert(sizeof(structure_bsp_material) == 0x100);
	struct structure_bsp_lightmap {
		short bitmap;
		unsigned short : 16;
		long:8 * sizeof(long) * 4;
		Yelo::TagBlock<const structure_bsp_material> materials;
	};

	struct s_world_bounds {
		real_bounds x, y, z;
	};

	struct structure_subcluster {
		s_world_bounds             world_bounds;
		Yelo::TagBlock<long> surface_indices;
	};

	struct structure_mirror {
		real_plane3d plane;
		long:8 * sizeof(long) * 5;
		tag_reference                      shader;
		Yelo::TagBlock<const real_point3d> vertices;
	};
	struct structure_cluster {
		short sky;
		short fog;
		short background_sound; // block index
		short sound_environment; // block index
		short weather; // block index
		short transition_structure_bsp;
		long: 32 * 6;
		Yelo::TagBlock<const predicted_resource>   predicted_resources;
		Yelo::TagBlock<const structure_subcluster> subclusters;
		short                                      first_lens_flare_marker_index;
		short                                      lens_flare_marker_count;
		Yelo::TagBlock<const long>                 surface_indices;
		tag_block                                  mirrors;
		tag_block                                  portals;
	}; static_assert(sizeof(structure_cluster) == 0x68);

	struct s_structure_fog_plane {
		short                               front_region;
		short                               runtime_material_type; // NONE or _material_type_water
		real_plane3d                        plane;
		Yelo::TagBlock<const real_vector3d> vertices;
	};
	struct s_structure_fog_region {
		long:8 * sizeof(long) * 9;
		short fog_palette; // block index
		short weather_palette; // block index
	};
	struct s_structure_fog_palette {
		tag_string    name;
		tag_reference fog;
		long:8 * sizeof(long) * 1;
		tag_string fog_scale_function;
		long:8 * sizeof(long) * 11 + 2;
	};
	struct structure_weather_palette_entry {
		tag_string    name;
		tag_reference particle_system;
		long:8 * sizeof(long) * 1;
		tag_string particle_system_scale_function;
		long:8 * sizeof(long) * 11;
		tag_reference wind;
		real_vector3d wind_direction;
		real          wind_magnitude;
		long:8 * sizeof(long) * 1;
		tag_string wind_scale_function;
		long:8 * sizeof(long) * 11;
	};

	struct structure_background_sound_palette_entry {
		tag_string    name;
		tag_reference background_sound;
		long:8 * sizeof(long) * 1;
		tag_string scale_function;
		tag_string:8 * sizeof(tag_string) * 1;
	};
	struct structure_sound_environment_palette_entry {
		tag_string    name;
		tag_reference sound_environment;
		tag_string:8 * sizeof(tag_string) * 1;
	};

	// TODO: move to structures\leaf_map.hpp
	struct s_leaf_map {
		TagGroups::collision_bsp *collision; // initialized by postprocess proc, only valid in tags builds
		tag_block                leaves;
		tag_block                portals;
	}; static_assert(sizeof(s_leaf_map) == 0x1C);

	struct structure_bsp {
		enum { k_group_tag = 'sbsp' };

		tag_reference lightmap_bitmaps;
		real_bounds   vehicle_heights;
		long:8 * sizeof(long) * 5; // Useless? Removed in H2

		Objects::s_object_lighting default_lighting; // doesn't expose distant_light_count
		unsigned long : 32; // Useless? Wasn't removed in H2, but can't find any code references in H1

		Yelo::TagBlock<const structure_collision_material> collision_materials;
		Yelo::TagBlock<const collision_bsp>                collision_bsp;
		Yelo::TagBlock<const structure_node>               nodes;

		struct {
			real_bounds x, y, z;
		}                                                  world_bounds;

		Yelo::TagBlock<const structure_leaf>              leafs;
		Yelo::TagBlock<const structure_surface_reference> leaf_surfaces;
		Yelo::TagBlock<const structure_surface>           surfaces;

		Yelo::TagBlock<const structure_bsp_lightmap> lightmaps;
		tag_block                                    bsp_padA; // Useless? Removed in H2

		tag_block lens_flares;
		tag_block lens_flare_markers;

		Yelo::TagBlock<structure_cluster> clusters;
		tag_data                          cluster_data;
		tag_block                         cluster_portals;
		tag_block                         bsp_padB; // Useless? Removed in H2

		tag_block                                     breakable_surfaces;
		tag_block                                     fog_planes;
		Yelo::TagBlock<const s_structure_fog_region>  fog_regions;
		Yelo::TagBlock<const s_structure_fog_palette> fog_palette;
		tag_block                                     bsp_padC; // Useless? Removed in H2
		tag_block                                     bsp_padD; // Useless? Removed in H2

		Yelo::TagBlock<const structure_weather_palette_entry> weather_palette;
		tag_block                                             weather_polyhedra;
		tag_block                                             bsp_padE; // Useless? Removed in H2 (new block was added)
		tag_block                                             bsp_padF; // Useless? Removed in H2 (new block was added)

		Yelo::TagBlock<const byte> pathfinding_surfaces;
		Yelo::TagBlock<const byte> pathfinding_edges;

		Yelo::TagBlock<const structure_background_sound_palette_entry>  background_sound_palette;
		Yelo::TagBlock<const structure_sound_environment_palette_entry> sound_environment_palette;
		tag_data                                                        sound_pas_data;
		tag_block                                                       bsp_padG; // Useless? Removed in H2
		tag_block                                                       bsp_padH; // Useless? Removed in H2

		tag_block markers;
		tag_block detail_objects;
		tag_block runtime_decals;
		unsigned __int64 : 64; // Useless? Removed in H2
		s_leaf_map leaf_map;

	}; static_assert(sizeof(structure_bsp) == 0x288);

	struct structure_bsp_header {
		structure_bsp *bsp;
		//////////////////////////////////////////////////////////////////////////
		// only used\initialized for Xbox targets
		struct {
			long count;
			void *address;
		}             vertex_buffer, lightmap_vertex_buffer;

		tag signature;
	}; static_assert(sizeof(structure_bsp_header) == 0x18);
};
