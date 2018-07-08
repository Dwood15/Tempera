#pragma once
#include "../../cseries/base.h"
#include "../../math/int_math.h"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_rasterizer_maximum_dynamic_triangles = 0x8000,

			k_max_dynamic_vertex_buffers = 0x100,
		};

		enum {
			_geometry_detail_level_super_low,
			_geometry_detail_level_low,
			_geometry_detail_level_medium,
			_geometry_detail_level_high,
			_geometry_detail_level_super_high,

			k_number_of_geometry_detail_levels
		};

		enum triangle_buffer_type : short
		{
			_triangle_buffer_type_0,
			_triangle_buffer_type_precompiled_strip,

			k_number_of_triangle_buffer_types
		};

		enum rasterizer_vertex_type : short
		{
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
	};

	namespace Rasterizer
	{
		struct rasterizer_triangle
		{
			short vertex0_index;
			short vertex1_index;
			short vertex2_index;
		}; static_assert(sizeof(rasterizer_triangle) == 0x6);

		struct rasterizer_triangle_buffer
		{
			Enums::triangle_buffer_type type;
			unsigned short : 16;
			long count;
			void* data;
			void* hardware_format;
		}; static_assert( sizeof(rasterizer_triangle_buffer) == 0x10 );

		struct rasterizer_vertex_buffer
		{
			Enums::rasterizer_vertex_type type;
			unsigned short : 16;
			long vertex_start_index;
			long vertex_count;
			void* data;
			void* hardware_format;
		}; static_assert( sizeof(rasterizer_vertex_buffer) == 0x14 );

		struct environment_vertex_uncompressed
		{
			real_point3d position;
			real_vector3d normal, binormal;
			real_vector3d tangent;
			real_point2d texcoord;
		};
		struct environment_vertex_compressed
		{
			real_point3d position;
			uint normal, binormal;
			uint tangent;
			real_point2d texcoord;
		};

		struct environment_lightmap_vertex_uncompressed
		{
			real_vector3d normal;
			real_point2d texcoord;
		};
		struct environment_lightmap_vertex_compressed
		{
			uint normal;
			point2d texcoord;
		};

		struct model_vertex_uncompressed
		{
			real_point3d position;
			real_vector3d normal, binormal;
			real_vector3d tangent;
			real_point2d texcoord;

			short node_indices[2];
			real node_weights[2];
		};
		struct model_vertex_compressed
		{
			real_point3d position;
			uint normal, binormal;
			uint tangent;
			point2d texcoord;

			byte node_indices[2];
			unsigned short node_weights[2];
		};
	};
};
