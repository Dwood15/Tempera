#pragma once

namespace Yelo::Enums {
    constexpr unsigned int k_rasterizer_maximum_dynamic_triangles = 0x8000;
	constexpr unsigned int k_max_dynamic_vertex_buffers = 0x100;

	enum {
		_geometry_detail_level_super_low,
		_geometry_detail_level_low,
		_geometry_detail_level_medium,
		_geometry_detail_level_high,
		_geometry_detail_level_super_high,

		k_number_of_geometry_detail_levels
	};

	enum triangle_buffer_type : short {
		_triangle_buffer_type_0,
		_triangle_buffer_type_precompiled_strip,

		k_number_of_triangle_buffer_types
	};

	enum rasterizer_vertex_type : short {
		_rasterizer_vertex_type_environment_uncompressed,
		_rasterizer_vertex_type_environment_compressed,

		_rasterizer_vertex_type_environment_lightmap_uncompressed,
		_rasterizer_vertex_type_environment_lightmap_compressed,

		_rasterizer_vertex_type_model_uncompressed,
		_rasterizer_vertex_type_model_compressed,

		_rasterizer_vertex_type_unlit,
		_rasterizer_vertex_type_dynamic_unlit,
		_rasterizer_vertex_type_dynamic_screen,
		_rasterizer_vertex_type_debug,
		_rasterizer_vertex_type_decal,
		_rasterizer_vertex_type_detail_object,

		_rasterizer_vertex_type_environment_uncompressed_ff,
		_rasterizer_vertex_type_environment_lightmap_uncompressed_ff,
		_rasterizer_vertex_type_model_uncompressed_ff,
		_rasterizer_vertex_type_model_processed,

		_rasterizer_vertex_type_unlit_zsprite,
		_rasterizer_vertex_type_screen_transformed_lit,
		_rasterizer_vertex_type_screen_transformed_lit_specular,

		_rasterizer_vertex_type_environment_single_stream_ff,

		k_number_of_rasterizer_vertex_types,
	};

    constexpr unsigned int k_maximum_rendered_objects = 256;
    constexpr unsigned int k_maximum_rendered_clusters = 0x80;
	constexpr unsigned int k_maximum_rendered_triangles = 0x4000;
	constexpr unsigned int k_maximum_rendered_distant_lights = 2;
	constexpr unsigned int k_maximum_rendered_point_lights   = 2;

	enum {
		k_maximum_rendered_objects_upgrade_amount = 256,
	};
}
