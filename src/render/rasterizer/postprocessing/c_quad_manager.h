#pragma once

#include <winnt.h>
#include <enums/postprocessing.h>
#include "../dx9/shaders/shader_postprocess_definitions.hpp"
#include "../../../memory/linked_list.hpp"
#include "Interfaces/IPostProcessingCacheComponent.hpp"

namespace Yelo::Rasterizer::PostProcessing {
	// commented out texcoords are for vertex data which may or may not be used to pass custom values in future
	const DWORD kVertexFormat =
						D3DFVF_XYZ | D3DFVF_TEX4 |
						D3DFVF_TEXCOORDSIZE4(0) |
						D3DFVF_TEXCOORDSIZE4(1) |
						D3DFVF_TEXCOORDSIZE4(2) |
						D3DFVF_TEXCOORDSIZE4(3)/* |
			D3DFVF_TEXCOORDSIZE2(4) |
			D3DFVF_TEXCOORDSIZE2(5) |
			D3DFVF_TEXCOORDSIZE2(6) |
			D3DFVF_TEXCOORDSIZE2(7)*/;

	struct s_postprocess_vertex {
		real_point3d    m_position;
		real_quaternion m_texcoord_private[4];
		//real_point2d m_texcoord_custom[4];

		s_postprocess_vertex() {
			memset(&m_position, 0, sizeof(m_position));
			memset(&m_texcoord_private, 0, sizeof(m_texcoord_private));
			//memset(&m_texcoord_custom, 0, sizeof(m_texcoord_custom));
		}
	};

	class c_quad_instance : public LinkedListNode<c_quad_instance> {
		/////////////////////////////////////////////////
		// members
	protected:
		uint32 m_reference_count;
		struct s_quad {
			point2d     tessellation;
			real_bounds x_bounds;
			real_bounds y_bounds;
			int32       vertex_count;
			int32       primitive_count;
			int32       start_vertex;
			int32       start_index;
		}      m_quad;

		/////////////////////////////////////////////////
		// member accessors
	public:
		s_quad &Quad();

		/////////////////////////////////////////////////
		// initializers
	public:
		void Ctor(const point2d tesselation, const real_bounds &x_bounds, const real_bounds &y_bounds) {
			m_quad.tessellation = tesselation;
			memcpy(&m_quad.x_bounds, &x_bounds, sizeof(m_quad.x_bounds));
			memcpy(&m_quad.y_bounds, &y_bounds, sizeof(m_quad.y_bounds));
			m_quad.vertex_count    = (m_quad.tessellation.x + 1) * (m_quad.tessellation.y + 1);
			m_quad.primitive_count = (m_quad.tessellation.x * m_quad.tessellation.y) * 2;
			m_quad.start_vertex    = 0;
			m_quad.start_index     = 0;
			ClearNodeData();
			m_reference_count = 0;
		}

		void Dtor() {
			m_quad.tessellation.x  = m_quad.tessellation.y = 0;
			m_quad.x_bounds.lower  = m_quad.x_bounds.upper = 0;
			m_quad.y_bounds.lower  = m_quad.y_bounds.upper = 0;
			m_quad.vertex_count    = 0;
			m_quad.primitive_count = 0;
			m_quad.start_vertex    = 0;
			m_quad.start_index     = 0;
			ClearNodeData();
			m_reference_count = 0;
		}

		/////////////////////////////////////////////////
		// application
	public:
		uint32 GetRef();

		void AddRef();

		void Release();
	};

	struct c_quad_manager : public IPostProcessingCacheComponent {
		/////////////////////////////////////////////////
		// static members
	private:
		static c_quad_manager g_quad_manager;

		/////////////////////////////////////////////////
		// static member accessors
	public:
		static c_quad_manager &Instance() { return g_quad_manager; }

	private:
		enum {
			k_maximum_quads_per_axis = 20, // maximum number of quads allowed on each axis
		};

		struct s_face {
			int16 m_indices[3];
		};

		/////////////////////////////////////////////////
		// members
		struct s_quad_manager_globals {
			struct {
				bool is_disabled;
				char : 8;
			}                          m_flags;
			Enums::pp_component_status status;

			// head of quad list
			c_quad_instance *m_quad_list;

			struct {
				LPDIRECT3DVERTEXBUFFER9 vertex;
				LPDIRECT3DINDEXBUFFER9  index;
			}               m_buffers;
		}                     m_globals;

		/////////////////////////////////////////////////
		// member accessors
	public:
		bool IsDisabled();

		Enums::pp_component_status ComponentStatus();

		/////////////////////////////////////////////////
		// IPostProcessingComponent
	public:
		void Initialize();

		void Dispose();

		void InitializeResources_Base(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *parameters);

		void OnLostDevice_Base();

		void OnResetDevice_Base(D3DPRESENT_PARAMETERS *parameters);

		void ReleaseResources_Base();

		/////////////////////////////////////////////////
		// IPostProcessingCacheComponent
	public:
		void InitializeResources_Cache();

		void ReleaseResources_Cache();

		/////////////////////////////////////////////////
		// system setup
	private:
		void CullUnusedQuads();

		bool CreateBuffersImpl();

		void DestroyBuffersImpl();

		c_quad_instance *GetExistingQuad(const point2d tesselation, const real_bounds &x_bounds, const real_bounds &y_bounds);

		int32 GetVertexCount();

		int32 GetIndexCount();

		void AddVertices(s_postprocess_vertex *&buffer_pointer,
							  c_quad_instance *quad,
							  const point2d dimensions);

		void AddIndices(s_face *&buffer_pointer, c_quad_instance *quad);

	public:
		bool CreateBuffers();

		void DestroyBuffers();

		c_quad_instance *CreateQuad(const TagGroups::s_effect_postprocess_quad_definition &);

		/////////////////////////////////////////////////
		// system application
	public:
		HRESULT SetBuffers();
	};
};
