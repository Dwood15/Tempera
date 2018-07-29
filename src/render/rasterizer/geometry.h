#pragma once

#include "macros_generic.h"
#include "../../math/int_math.h"
#include "../../math/real_math.h"

namespace Yelo::Rasterizer {
	struct rasterizer_triangle {
		short vertex0_index;
		short vertex1_index;
		short vertex2_index;
	}; static_assert(sizeof(rasterizer_triangle) == 0x6);

	struct rasterizer_triangle_buffer {
		Enums::triangle_buffer_type type;
		unsigned short : 16;
		long count;
		void *data;
		void *hardware_format;
	}; static_assert(sizeof(rasterizer_triangle_buffer) == 0x10);

	struct rasterizer_vertex_buffer {
		Enums::rasterizer_vertex_type type;
		unsigned short : 16;
		long vertex_start_index;
		long vertex_count;
		void *data;
		void *hardware_format;
	}; static_assert(sizeof(rasterizer_vertex_buffer) == 0x14);

	struct environment_vertex_uncompressed {
		real_point3d  position;
		real_vector3d normal, binormal;
		real_vector3d tangent;
		real_point2d  texcoord;
	};
	struct environment_vertex_compressed {
		real_point3d position;
		uint         normal, binormal;
		uint         tangent;
		real_point2d texcoord;
	};

	struct environment_lightmap_vertex_uncompressed {
		real_vector3d normal;
		real_point2d  texcoord;
	};
	struct environment_lightmap_vertex_compressed {
		uint    normal;
		point2d texcoord;
	};

	struct model_vertex_uncompressed {
		real_point3d  position;
		real_vector3d normal, binormal;
		real_vector3d tangent;
		real_point2d  texcoord;

		short node_indices[2];
		real  node_weights[2];
	};
	struct model_vertex_compressed {
		real_point3d position;
		uint         normal, binormal;
		uint         tangent;
		point2d      texcoord;

		byte           node_indices[2];
		unsigned short node_weights[2];
	};
};
