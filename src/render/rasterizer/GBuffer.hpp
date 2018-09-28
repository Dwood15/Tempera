/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum render_progress {
			_render_progress_sky,
			_render_progress_objects,
			_render_progress_objects_transparent,
			_render_progress_structure,
			_render_progress_none,
		};
	};

	namespace DX9
	{
		enum {
			k_maximum_multi_render_target = 3,
		};
		//////////////////////////////////////////////////////////////////////
		// Container for holding all members of the GBuffer					//
		//////////////////////////////////////////////////////////////////////
		struct c_gbuffer
		{
			enum {
				k_gbuffer_depth_x = 0,		//R32_F		X

				k_gbuffer_velocity_x = 0,	//R16_F		X
				k_gbuffer_velocity_y = 1,	//R16_F			Y

				k_gbuffer_normals_x = 0,	//R8G8B8A8	X
				k_gbuffer_normals_y = 1,	//R8G8B8A8		Y

				k_gbuffer_index_x = 2,		//R8G8B8A8			Z
				k_gbuffer_index_y = 3,		//R8G8B8A8				W
			};
			Render::s_render_target					m_rt_depth;
			Render::s_render_target					m_rt_velocity;
			Render::s_render_target					m_rt_normals_index;

			void ReleaseTargets();
			void ClearTargets(IDirect3DDevice9* pDevice);
			bool SetDepth(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetVelocity(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetNormals(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetIndex(LPD3DXEFFECT& effect, bool& variable_used);
			
			bool SetEffectVar(LPD3DXEFFECT& effect,
				bool& variable_used,
				cstring texture_semantic,
				Render::s_render_target& target,
				cstring x_handle_semantic = NULL, const int x_index = 0,
				cstring y_handle_semantic = NULL, const int y_index = 0,
				cstring z_handle_semantic = NULL, const int z_index = 0,
				cstring w_handle_semantic = NULL, const int w_index = 0);
		};

		struct s_render_target_output
		{
			enum { k_target_count = 4, };

			LPDIRECT3DSURFACE9 m_targets[k_target_count];

			LPDIRECT3DSURFACE9& s_render_target_output::operator [] (int32 index)
			{
				return m_targets[index];
			}
		};

		class c_gbuffer_fullscreen_effect
		{
		protected:
			LPD3DXEFFECT	m_effect;
			TEXTURE_VERTEX	m_vertices[4];

		public:
			virtual HRESULT	AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			OnLostDevice();
			HRESULT			OnResetDevice();
			virtual void	Render(IDirect3DDevice9* device) {}

			virtual bool	IsAvailable();
			LPD3DXEFFECT&	GetEffect() { return m_effect; }	
		};

		class c_gbuffer_debug_effect : public c_gbuffer_fullscreen_effect
		{
			D3DXHANDLE		m_technique_single;
			D3DXHANDLE		m_technique_all;

			D3DXHANDLE		m_far_clip_handle;
			D3DXHANDLE		m_target_handle;

			bool			m_depth_set;
			bool			m_velocity_set;
			bool			m_normals_set;
			bool			m_index_set;
		public:
			HRESULT			AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			Render(IDirect3DDevice9* device, int16 debug_target);

			bool			IsAvailable();	
		};
		static c_gbuffer_debug_effect& GBufferDebug();

		class c_gbuffer_rtclear_effect : public c_gbuffer_fullscreen_effect
		{
		public:
			struct {
				D3DXHANDLE clear_technique;
				s_render_target_output output[k_maximum_multi_render_target];
				uint32 count;
			}m_multi_rt;

			HRESULT			AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			Render(IDirect3DDevice9* device);

			bool			IsAvailable();	
		};
		static c_gbuffer_rtclear_effect& GBufferClear();

		class c_gbuffer_system
		{
		public:
			static int16					g_debug_index;
			static bool						g_system_enabled;

		private:
			static Enums::rasterizer_vertex_shader const kValidShaders[];

			static Enums::render_progress	g_current_render_state;
			
			static BOOL						g_output_object_tbn;
			static BOOL						g_output_object_velocity;
			static BOOL						g_output_velocity;
			static D3DXVECTOR4				g_pixel_shader_input;
			
			static D3DXMATRIX				g_identity_matrix;
			static BOOL						g_wvp_stored;
			static D3DXMATRIX				g_stored_worldviewproj[2];
			static BOOL						g_stored_wvp_index;

			static datum_index				g_object_index;
			static void Hook_RenderObjectList_GetObjectIndex();
			static void Hook_RenderObjectList_ClearObjectIndex();
			static void Hook_FirstPersonWeaponDraw_GetObjectIndex();
			
			static uint32					g_current_object_lod;
			static void Hook_RenderObject_GetCurrentLOD();

			static void Hook_CommandCameraSet();
			static void Hook_CommandSwitchBSP();
			static void Hook_CommandGameSave();

			static void Hook_RenderObjectsTransparent();
		private:
			c_packed_file			m_shader_package;
			bool					m_is_loaded;
			bool					m_render_gbuffer;
			PAD16;
			LPD3DXEFFECT			m_gbuffer_ps, m_gbuffer_vs;
			c_gbuffer				m_gbuffer;

			struct {
				s_render_target_output output[k_maximum_multi_render_target];
				uint32 count;
			}m_multi_rt;

			struct {
				struct {
					D3DXHANDLE n;
					D3DXHANDLE n_v;
				}ps_bsp_techniques;
				struct {
					D3DXHANDLE n;
					D3DXHANDLE n_v;
				}vs_bsp_techniques;

				struct {
					D3DXHANDLE n;
					D3DXHANDLE tbn;
					D3DXHANDLE n_v;
					D3DXHANDLE tbn_v;
					D3DXHANDLE none;
				}ps_object_techniques;
				struct {
					D3DXHANDLE n;
					D3DXHANDLE tbn;
					D3DXHANDLE n_v;
					D3DXHANDLE tbn_v;
					D3DXHANDLE none;
				}vs_object_techniques;
			}m_structures;

			bool IsRenderable() const
			{
				return m_is_loaded && m_render_gbuffer && g_system_enabled;
			}
		public:

			void Ctor();
			void Dtor();
			// Hooks the non-reflection render call and opens the shader file 
			static void	Initialize();
			// Nothing
			static void	Dispose();
			// Swap the stored WVP's
			static void Update(real delta_time);
					
			// Calls AllocateResources
			static void	Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			// Calls on lost device for the gbuffer shaders then releases resources
			static void	OnLostDevice();
			// Allocates resources, then calls on reset device for the GBuffer shaders
			static void	OnResetDevice(D3DPRESENT_PARAMETERS* params);
			// Nothing
			static void	Render();
			// Release direct3D resources when quitting
			static void Release();
			// Exposes g_output_object_tbn
			static BOOL& OutputObjectTBN();

			static bool& RenderGBuffer();

		public:
			// Called for each mesh drawn to render to the GBuffer
			static HRESULT	 	DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Structure(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Object(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);			
			static bool			SetViewProj(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount);
			// Clears all GBuffer surfaces, ready for the next frame
			static void			ClearGBuffer(IDirect3DDevice9* pDevice);

		private:
			static c_gbuffer_system			g_default_system;

			void		OnLostDeviceImpl();
			void		OnResetDeviceImpl(D3DPRESENT_PARAMETERS* params);
			// Creates GBuffer textures and shaders
			void		AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* params);
			// Releases GBuffer textures and shaders
			void		ReleaseResources();
			// Called for each mesh drawn to render to the GBuffer
			HRESULT	 	DrawIndexedPrimitive_Impl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			HRESULT	 	DrawIndexedPrimitive_StructureImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			HRESULT	 	DrawIndexedPrimitive_ObjectImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			// Clears all GBuffer surfaces, ready for the next frame
			void		ClearGBufferImpl(IDirect3DDevice9* pDevice);
			// Renders a full screen quad with one or all GBuffer textures
			void		RenderDebugQuad(IDirect3DDevice9* pDevice);
			// Loads an effect from a packed file
			HRESULT		LoadEffect(IDirect3DDevice9* pDevice, LPD3DXEFFECT* pEffect, const char* EffectID);
			bool&		RenderGBufferImpl();

		public:
			static c_gbuffer&				GBuffer();

			struct render_progress
			{
				static void SkyPreProcess();
				static void SkyPostProcess();

				static void ObjectsPreProcess();
				static void ObjectsPostProcess();

				static void StructurePreProcess();
				static void StructurePostProcess();

				static void UIPreProcess();
			};
		};
	};
};
#endif