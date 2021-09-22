#pragma once

#include "collision_bsp.h"
#include "../render/rasterizer/geometry.h"

namespace Yelo::TagGroups {
	using namespace Yelo;
	
	struct tag_block_int {
		long count;
		int *Address;
		Yelo::tag_block_definition *tagBlockDefinition;
	}; STAT_ASSERT(tag_block_int, 0xC);	
	
	struct tag_block_unk {
		long count;
		void *Address;
		Yelo::tag_block_definition *tagBlockDefinition;
	}; STAT_ASSERT(tag_block_unk, 0xC);

	struct structure_surface {
		short a[3];
	};

	struct structure_bsp_material {
		s_tag_ref              shader;
		short                      shader_permutation;
		unsigned short             flags;
		long                       surfaces;
		long                       surface_count;
		real_point3d               centroid;
		Objects::s_object_lighting object_lighting;
		real_plane3d               plane;
		short                      breakable_surface;
		unsigned short unkPad0;

		Rasterizer::rasterizer_vertex_buffer vertices;
		Rasterizer::rasterizer_vertex_buffer lightmap_vertices;
		tag_data uncompressed_vertices;
		tag_data compressed_vertices;
	}; static_assert(sizeof(structure_bsp_material) == 0x100);

	struct structure_bsp_lightmap {
		short                                  bitmap;
		short                                  padA;
		long                                   padB[4];
		Yelo::TagBlock<structure_bsp_material> materials;
	};

	STAT_ASSERT(structure_bsp_lightmap, 0x14 + sizeof(Yelo::TagBlock<structure_bsp_material>));

	struct s_world_bounds {
		real_bounds x, y, z;
	};

	struct structure_subcluster {
		s_world_bounds       world_bounds;
		Yelo::TagBlock<long> surface_indices;
	};

	struct structure_mirror {
		real_plane3d                       plane;
		long                               padA[5];
		s_tag_ref                      shader;
		Yelo::TagBlock<const real_point3d> vertices;
	};

	struct predicted_resource {
		short       type;
		short       resource_index;
		datum_index tag_index;
	};

	STAT_ASSERT(predicted_resource, 0x8);

	struct structure_cluster {
		short                                sky;
		short                                fog;
		short                                background_sound; // block index
		short                                sound_environment; // block index
		short                                weather; // block index
		short                                transition_structure_bsp;
		long                                 padA[7];
		// Yelo::TagBlock<predicted_resource>   predicted_resources;
		Yelo::TagBlock<predicted_resource>   predicted_resources;
		Yelo::TagBlock<structure_subcluster> subclusters;
		short                                first_lens_flare_marker_index;
		short                                lens_flare_marker_count;
		tag_block_int                 surface_indices;
		tag_block_int                       mirrors;
		tag_block_int                       portals;
	};

	STAT_ASSERT(structure_cluster, 0x68);

	struct s_structure_fog_plane {
		short                               front_region;
		short                               runtime_material_type; // NONE or _material_type_water
		real_plane3d                        plane;
		Yelo::TagBlock<const real_vector3d> vertices;
	};

	struct s_structure_fog_region {
		long  padA[9];
		short fog_palette; // block index
		short weather_palette; // block index
	};

	struct s_structure_fog_palette {
		tag_string    name;
		s_tag_ref fog;
		long          padA;
		tag_string    fog_scale_function;
		long          padB[13];
	};
	struct structure_weather_palette_entry {
		tag_string    name;
		s_tag_ref particle_system;
		long          padA;
		tag_string    particle_system_scale_function;
		long          padB[11];
		s_tag_ref wind;
		real_vector3d wind_direction;
		real          wind_magnitude;
		long          padC;
		tag_string    wind_scale_function;
		long          padD[11];
	};

	struct structure_background_sound_palette_entry {
		tag_string    name;
		s_tag_ref background_sound;
		long          padA;
		tag_string    scale_function;
		tag_string    padB;
	};

	struct structure_sound_environment_palette_entry {
		tag_string    name;
		s_tag_ref sound_environment;
		tag_string    padA;
	};

	STAT_ASSERT(structure_sound_environment_palette_entry, sizeof(tag_string) * 2 + sizeof(s_tag_ref));

	// TODO: move to structures\leaf_map.hpp
	struct s_leaf_map {
		Yelo::TagGroups::coll::collision_bsp *collision; // initialized by postprocess proc, only valid in tags builds
		tag_block_unk                    leaves;
		tag_block_unk                    portals;
	};

	STAT_ASSERT(s_leaf_map, 0x1C);

	STAT_ASSERT(Yelo::TagBlock<structure_leaf>, 0xC);
	STAT_ASSERT(Yelo::TagBlock<structure_surface_reference>, 0xC);
	STAT_ASSERT(Yelo::TagBlock<structure_surface>, 0xC);
	STAT_ASSERT(tag_block_unk, 0xC);

#pragma pack(push, 1)
	struct structure_bsp {
		s_tag_ref lightmap_bitmaps; //0x0
		real_bounds   vehicle_heights; //0x10 //floor, ceiling
		long          padA[5]; // Useless? Removed in H2

		Objects::s_object_lighting default_lighting; // doesn't expose distant_light_count
		unsigned long              pad00; // Useless? Wasn't removed in H2, but can't find any code references in H1

		Yelo::TagBlock<structure_collision_material>         collision_materials;
		Yelo::TagBlock<Yelo::TagGroups::coll::collision_bsp> collision_bsp;
		Yelo::TagBlock<structure_node>                       nodes;

		struct {
			real_bounds x, y, z;
		}                                                    world_bounds;

		Yelo::TagBlock<structure_leaf>              leafs;
		Yelo::TagBlock<structure_surface_reference> leaf_surfaces;
		Yelo::TagBlock<structure_surface>           surfaces;

		Yelo::TagBlock<structure_bsp_lightmap> lightmaps;
		tag_block_unk                      bsp_padA; // Useless? Removed in H2

		tag_block_unk lens_flares;
		tag_block_unk lens_flare_markers;

		Yelo::TagBlock<structure_cluster> clusters;
		tag_data                          cluster_data;
		tag_block_unk                 cluster_portals;
		tag_block_unk                 bsp_padB; // Useless? Removed in H2

		tag_block_unk                       breakable_surfaces;
		tag_block_unk                       fog_planes;
		Yelo::TagBlock<s_structure_fog_region>  fog_regions;
		Yelo::TagBlock<s_structure_fog_palette> fog_palette;
		tag_block_unk                       bsp_padC; // Useless? Removed in H2
		tag_block_unk                       bsp_padD; // Useless? Removed in H2

		Yelo::TagBlock<structure_weather_palette_entry> weather_palette;
		tag_block_unk weather_polyhedra;
		tag_block_unk bsp_padE; // Useless? Removed in H2 (new block was added)
		tag_block_unk bsp_padF; // Useless? Removed in H2 (new block was added)

		Yelo::TagBlock<byte> pathfinding_surfaces;
		Yelo::TagBlock<byte> pathfinding_edges;

		Yelo::TagBlock<structure_background_sound_palette_entry>  background_sound_palette;
		Yelo::TagBlock<structure_sound_environment_palette_entry> sound_environment_palette;

		tag_data sound_pas_data;

		tag_block_unk bsp_padG; // Useless? Removed in H2
		tag_block_unk bsp_padH; // Useless? Removed in H2

		tag_block_unk markers;
		tag_block_unk detail_objects;
		tag_block_unk runtime_decals;

		unsigned long long  PadLast; // Useless? Removed in H2
		s_leaf_map        leaf_map;

	};
#pragma pack(pop)
	STAT_ASSERT(structure_bsp, 0x288);

	struct structure_bsp_header {
		structure_bsp *bsp;
		//////////////////////////////////////////////////////////////////////////
		// only used\initialized for Xbox targets
		struct {
			long count;
			void *address;
		}             vertex_buffer, lightmap_vertex_buffer;

		tag signature;
	};

	STAT_ASSERT(structure_bsp_header, 0x18);
};
