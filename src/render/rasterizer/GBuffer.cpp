/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/game/game_time_structures.hpp>
#include <blamlib/Halo1/game/player_structures.hpp>
#include <blamlib/Halo1/main/main_structures.hpp>

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Settings/Settings.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/PostProcessing/PostProcessingErrorReporting.hpp"
#include "Game/GameEngine.hpp"
#include "Game/GameState.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/Players.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_GBUFFER
#include "Memory/_EngineLayout.inl"

#pragma region Settings
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<bool> m_enabled;

			c_settings_container()
				: Configuration::c_configuration_container("Rasterizer.GBuffer")
				, m_enabled("Enabled", true)
			{ }
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override
			{
				return std::vector<i_configuration_value* const> { &m_enabled };
			}
		};

		class c_settings_gbuffer
			: public Settings::c_settings_singleton<c_settings_container, c_settings_gbuffer>
		{
		public:
			void PostLoad() final override
			{
				c_gbuffer_system::g_system_enabled = Get().m_enabled;
			}

			void PreSave() final override
			{
				Get().m_enabled = c_gbuffer_system::g_system_enabled;
			}
		};
#pragma endregion

		static D3DXHANDLE	GetTechnique(LPD3DXEFFECT& effect,
			const char* mesh_type, 
			const char* rt_support)
		{
			const char* technique_format = "VS_%s_%s";
			char technique[32] = "";
			sprintf_s(technique, sizeof(technique), technique_format, mesh_type, rt_support);
			return effect->GetTechniqueByName(technique);
		}

		static D3DXHANDLE	GetTechnique(LPD3DXEFFECT& effect,
			const int rt_count,
			const char* mesh_type,
			const char* rt_support)
		{
			const char* technique_format = "PS_MRT%i_%s_%s";
			char technique[32] = "";
			sprintf_s(technique, technique_format, rt_count, mesh_type, rt_support);
			return effect->GetTechniqueByName(technique);
		}


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer
		void		c_gbuffer::ReleaseTargets()
		{
			m_rt_depth.ReleaseTarget();
			m_rt_velocity.ReleaseTarget();
			m_rt_normals_index.ReleaseTarget();
		}

		void		c_gbuffer::ClearTargets(IDirect3DDevice9* pDevice)
		{
			m_rt_depth.ClearTarget(pDevice, NULL, 1);
			m_rt_velocity.ClearTarget(pDevice, NULL, 1);
			m_rt_normals_index.ClearTarget(pDevice, NULL, 1);
		}
		bool		c_gbuffer::SetEffectVar(LPD3DXEFFECT& effect,
			bool& variable_used,
			cstring texture_semantic,
			Render::s_render_target& target,
			cstring x_handle_semantic, const int x_index,
			cstring y_handle_semantic, const int y_index,
			cstring z_handle_semantic, const int z_index,
			cstring w_handle_semantic, const int w_index)
		{
			variable_used = false;
			if(!effect) return false;

			//find the intended texture handle
			D3DXHANDLE tex_handle = effect->GetParameterBySemantic(nullptr, texture_semantic);

			variable_used = tex_handle != nullptr;

			if(!tex_handle)								return true;
			else if(tex_handle && !target.IsEnabled())	return false;

			// set the texture variable
			effect->SetTexture(tex_handle, target.texture);

			// search for the index variables handles
			// then set them to the indices for the intended data
			D3DXHANDLE index_handle = nullptr;
			if(x_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(nullptr, x_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, x_index);
			}
			if(y_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(nullptr, y_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, y_index);
			}
			if(z_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(nullptr, z_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, z_index);
			}
			if(w_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(nullptr, w_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, w_index);
			}
			
			return true;

		}
		bool		c_gbuffer::SetDepth(LPD3DXEFFECT& effect, bool& variable_used)
		{
			return SetEffectVar(effect, variable_used, "TEXDEPTH", m_rt_depth,
				"GBUFFER_DEPTH_X", k_gbuffer_depth_x);
		}

		bool		c_gbuffer::SetVelocity(LPD3DXEFFECT& effect, bool& variable_used)
		{
			return SetEffectVar(effect, variable_used, "TEXVELOCITY", m_rt_velocity,
				"GBUFFER_VELOCITY_X", k_gbuffer_velocity_x, 
				"GBUFFER_VELOCITY_Y", k_gbuffer_velocity_y);
		}
		bool		c_gbuffer::SetNormals(LPD3DXEFFECT& effect, bool& variable_used)
		{
			return SetEffectVar(effect, variable_used, "TEXNORMALS", m_rt_normals_index,
				"GBUFFER_NORMALS_X", k_gbuffer_normals_x,
				"GBUFFER_NORMALS_Y", k_gbuffer_normals_y);
		}
		bool		c_gbuffer::SetIndex(LPD3DXEFFECT& effect, bool& variable_used)
		{
			return SetEffectVar(effect, variable_used, "TEXINDEX", m_rt_normals_index,
				"GBUFFER_INDEX_X", k_gbuffer_index_x,
				"GBUFFER_INDEX_Y", k_gbuffer_index_y);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// gbuffer fullscreen effects
		static c_gbuffer_rtclear_effect g_gbuffer_clear;
		static c_gbuffer_debug_effect g_gbuffer_debug;

		c_gbuffer_rtclear_effect& GBufferClear() { return g_gbuffer_clear; }
		c_gbuffer_debug_effect& GBufferDebug() { return g_gbuffer_debug; }
		HRESULT		c_gbuffer_fullscreen_effect::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			TEXTURE_VERTEX quad[4] = 
			{
				{ -0.5f,			-0.5f,			1.0f, 1.0f, 0.0f, 0.0f },
				{ width - 0.5f,		-0.5,			1.0f, 1.0f, 1.0f, 0.0f },
				{ -0.5,				height - 0.5f,	1.0f, 1.0f, 0.0f, 1.0f },
				{ width - 0.5f,		height - 0.5f,	1.0f, 1.0f, 1.0f, 1.0f }
			};
			memcpy_s(m_vertices, sizeof(m_vertices), quad, sizeof(quad));

			return S_OK;
		}
		void		c_gbuffer_fullscreen_effect::ReleaseResources()
		{
			safe_release(m_effect);
		}
		void		c_gbuffer_fullscreen_effect::OnLostDevice()
		{
			if(m_effect)
				m_effect->OnLostDevice();
		}
		HRESULT		c_gbuffer_fullscreen_effect::OnResetDevice()
		{
			HRESULT hr = S_OK;
			if(m_effect)
				hr = m_effect->OnResetDevice();
			return hr;
		}
		bool		c_gbuffer_fullscreen_effect::IsAvailable()
		{
			return m_effect != nullptr;
		}

		HRESULT		c_gbuffer_debug_effect::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			c_gbuffer_fullscreen_effect::AllocateResources(device, width, height);

			m_technique_single =	m_effect->GetTechniqueByName("DebugRTSingle");
			m_technique_all =		m_effect->GetTechniqueByName("DebugRTAll");
			
			m_far_clip_handle =		m_effect->GetParameterByName(nullptr, "FarClipDistance");
			m_target_handle =		m_effect->GetParameterByName(nullptr, "RenderTarget");

			bool variable_used;
			m_depth_set =			c_gbuffer_system::GBuffer().SetDepth(m_effect, variable_used);
			m_velocity_set =		c_gbuffer_system::GBuffer().SetVelocity(m_effect, variable_used);
			m_normals_set =			c_gbuffer_system::GBuffer().SetNormals(m_effect, variable_used);
			m_index_set =			c_gbuffer_system::GBuffer().SetIndex(m_effect, variable_used);

			return (IsAvailable() ? S_OK : E_FAIL);
		}
		void		c_gbuffer_debug_effect::ReleaseResources()
		{
			c_gbuffer_fullscreen_effect::ReleaseResources();

			m_technique_single = nullptr;
			m_technique_all = nullptr;

			m_far_clip_handle = nullptr;
			m_target_handle = nullptr;

			m_depth_set = false;
			m_velocity_set = false;
			m_normals_set = false;
			m_index_set = false;
		}
		void		c_gbuffer_debug_effect::Render(IDirect3DDevice9* device, int16 debug_target)
		{
			if(!IsAvailable()) return;

			// save the current important render states so they can be restored later
			DWORD old_depthbias, old_fillmode, old_srcblend, old_dest_blend, old_zenable, old_zwriteenable, old_stencilenable;

			device->GetRenderState(D3DRS_DEPTHBIAS, &old_depthbias);
			device->GetRenderState(D3DRS_FILLMODE, &old_fillmode);
			device->GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
			device->GetRenderState(D3DRS_DESTBLEND, &old_dest_blend);
			device->GetRenderState(D3DRS_ZENABLE, &old_zenable);
			device->GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);
			device->GetRenderState(D3DRS_STENCILENABLE, &old_stencilenable);

			// set the render states that we need to draw the effect correctly
			device->SetRenderState(D3DRS_DEPTHBIAS, 0);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

			// draw the render target debug effect
			UINT cPasses, p;
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

			Render::s_render_target& rt = Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			device->SetRenderTarget(0, rt.surface);

			m_effect->SetTechnique(debug_target == NONE ?
				m_technique_all : m_technique_single);

			m_effect->SetFloat(m_far_clip_handle, Render::RenderGlobals()->frustum.z_far);
			m_effect->SetInt(m_target_handle, debug_target);

			m_effect->Begin(&cPasses, 0);
			for(p = 0; p < cPasses; ++p)
			{
				m_effect->BeginPass(p);
				device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertices, sizeof(TEXTURE_VERTEX));
				m_effect->EndPass();
			}
			m_effect->End();

			// restore the render states to their previous values
			device->SetRenderState(D3DRS_DEPTHBIAS, old_depthbias);
			device->SetRenderState(D3DRS_FILLMODE, old_fillmode);
			device->SetRenderState(D3DRS_SRCBLEND, old_srcblend);
			device->SetRenderState(D3DRS_DESTBLEND, old_dest_blend);
			device->SetRenderState(D3DRS_ZENABLE, old_zenable);
			device->SetRenderState(D3DRS_ZWRITEENABLE, old_zwriteenable);
			device->SetRenderState(D3DRS_STENCILENABLE, old_stencilenable);
		}
		bool		c_gbuffer_debug_effect::IsAvailable()
		{
			return (
				c_gbuffer_fullscreen_effect::IsAvailable() &&
				m_technique_single != nullptr &&
				m_technique_all != nullptr &&
				m_far_clip_handle != nullptr &&
				m_target_handle != nullptr &&
				m_depth_set &&
				m_velocity_set &&
				m_normals_set &&
				m_index_set
				);
		}

		HRESULT		c_gbuffer_rtclear_effect::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			c_gbuffer_fullscreen_effect::AllocateResources(device, width, height);

			m_multi_rt.clear_technique = GetTechnique(m_effect, m_multi_rt.count, "Clear", "ALL");

			return (IsAvailable() ? S_OK : E_FAIL);
		}
		void		c_gbuffer_rtclear_effect::ReleaseResources()
		{
			c_gbuffer_fullscreen_effect::ReleaseResources();

			m_multi_rt.clear_technique = nullptr;
		}
		void		c_gbuffer_rtclear_effect::Render(IDirect3DDevice9* device)
		{
			if(!IsAvailable()) return;

			DWORD old_fillmode, old_zwriteenable, old_alphablenenable, old_srcblend, old_destblend;

			device->GetRenderState(D3DRS_FILLMODE, &old_fillmode);
			device->GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);
			device->GetRenderState(D3DRS_ALPHABLENDENABLE, &old_alphablenenable);
			device->GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
			device->GetRenderState(D3DRS_DESTBLEND, &old_destblend);

			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

			UINT cPasses, p;
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

			m_effect->SetTechnique(m_multi_rt.clear_technique);

			m_effect->Begin(&cPasses, 0);
			for(p = 0; p < cPasses; ++p)
			{
				for(uint32 i = 0; i < m_multi_rt.count; i++)
					device->SetRenderTarget(i, m_multi_rt.output[p][i]);

				m_effect->BeginPass(p);
				device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertices, sizeof(TEXTURE_VERTEX));
				m_effect->EndPass();
				
				for(uint32 i = 1; i < m_multi_rt.count; i++)
					device->SetRenderTarget(i, nullptr);
			}
			m_effect->End();

			device->SetRenderState(D3DRS_FILLMODE, old_fillmode);
			device->SetRenderState(D3DRS_ZWRITEENABLE, old_zwriteenable);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alphablenenable);
			device->SetRenderState(D3DRS_SRCBLEND, old_srcblend);
			device->SetRenderState(D3DRS_DESTBLEND, old_destblend);
		}
		bool		c_gbuffer_rtclear_effect::IsAvailable()
		{
			return (
				c_gbuffer_fullscreen_effect::IsAvailable() &&
				m_multi_rt.clear_technique != nullptr
				);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system interface
		__declspec(naked) void c_gbuffer_system::Hook_RenderObjectList_GetObjectIndex()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_OBJECT_LIST_HOOK_RETN);

			__asm {
				mov		c_gbuffer_system::g_object_index, eax
				and		eax, 0FFFFh
				lea		edi, [eax+eax*2]
				jmp		RETN_ADDRESS
			}
		}

		__declspec(naked) void c_gbuffer_system::Hook_RenderObjectList_ClearObjectIndex()
		{
			__asm {
				mov		c_gbuffer_system::g_object_index, NONE//datum_index::null
				retn
			}
		}

		__declspec(naked) void c_gbuffer_system::Hook_FirstPersonWeaponDraw_GetObjectIndex()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(FIRST_PERSON_WEAPON_DRAW_HOOK_RETN);

			__asm {
				mov		edx, [edx+34h]
				mov		eax, esi
				mov		c_gbuffer_system::g_object_index, eax
				jmp		RETN_ADDRESS
			}
		}

		__declspec(naked) void c_gbuffer_system::Hook_RenderObject_GetCurrentLOD()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_OBJECT_OBJECT_LOD_HOOK_RETN);

			_asm{
				mov		ecx, [ebp+28h]
				mov		eax, [ebp+1Ch]

				push	eax
				mov		eax, [ebp-8]
				mov		c_gbuffer_system::g_current_object_lod, eax
				pop		eax

				jmp		RETN_ADDRESS
			};
		}

		// hooks for controlling when to disable velocity
		__declspec(naked) void c_gbuffer_system::Hook_CommandCameraSet()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_CAMERA_SET_HOOK_RETN);

			_asm{
				lea		edx, [esi+28h]
				mov		eax, [edx]

				push	eax
				mov		eax, [ebp+8]
				cmp		eax, 0x00000000
				jg		skip_disable_velocity
				mov		c_gbuffer_system::g_output_velocity, 0
skip_disable_velocity:
				pop		eax
				jmp		RETN_ADDRESS
			};
		}

		__declspec(naked) void c_gbuffer_system::Hook_CommandSwitchBSP()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_SWITCH_BSP_HOOK_RETN);

			_asm{
				cmp		si, dx
				mov		[esp+7], bl
				
				mov		c_gbuffer_system::g_output_velocity, 0

				jmp		RETN_ADDRESS
			};
		}

		__declspec(naked) void c_gbuffer_system::Hook_CommandGameSave()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_GAME_SAVE_HOOK_RETN);

			_asm{
				mov		c_gbuffer_system::g_output_velocity, 0

				mov		ecx, [esp+8]
				xor		eax, eax
				jmp		RETN_ADDRESS
			};
		}

		// the hooked function takes arguments, for which FunctionInterface is unsuited
		__declspec(naked) void c_gbuffer_system::Hook_RenderObjectsTransparent()
		{
			static const uintptr_t CALL_ADDRESS = GET_FUNC_PTR(RENDER_OBJECTS_TRANSPARENT);
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN);

			using namespace Enums;

			_asm{
				mov		c_gbuffer_system::g_current_render_state, _render_progress_objects_transparent
				call	CALL_ADDRESS
				mov		c_gbuffer_system::g_current_render_state, _render_progress_none
				jmp		RETN_ADDRESS
			}
		}

		void		c_gbuffer_system::Ctor()
		{
			m_shader_package.OpenFile("GBUF_GBuffer_Shaders_SHD", true);
		}
		void		c_gbuffer_system::Dtor()
		{
			m_shader_package.CloseFile();
		}
		void		c_gbuffer_system::Initialize()
		{
			g_default_system.Ctor();

			c_settings_gbuffer::Register(Settings::Manager());

			c_gbuffer_system::g_output_object_tbn = false;
			// leave as false, not enough vertex shader registers available to do object velocity with bones
			c_gbuffer_system::g_output_object_velocity = false;

			c_gbuffer_system::g_output_velocity = true;

			D3DXMatrixIdentity(&c_gbuffer_system::g_identity_matrix);

			Memory::WriteRelativeJmp(&Hook_RenderObjectList_GetObjectIndex,
				GET_FUNC_VPTR(RENDER_OBJECT_LIST_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_RenderObjectList_ClearObjectIndex,
				GET_FUNC_VPTR(RENDER_OBJECT_LIST_END_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_FirstPersonWeaponDraw_GetObjectIndex,
				GET_FUNC_VPTR(FIRST_PERSON_WEAPON_DRAW_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_RenderObject_GetCurrentLOD,
				GET_FUNC_VPTR(RENDER_OBJECT_OBJECT_LOD_HOOK), true);
			
			Memory::WriteRelativeJmp(&Hook_CommandCameraSet,
				GET_FUNC_VPTR(COMMAND_CAMERA_SET_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_CommandSwitchBSP,
				GET_FUNC_VPTR(COMMAND_SWITCH_BSP_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_CommandGameSave,
				GET_FUNC_VPTR(COMMAND_GAME_SAVE_HOOK), true);

			Memory::WriteRelativeJmp(&Hook_RenderObjectsTransparent,
				GET_FUNC_VPTR(RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK), true);

			byte NOP = Enums::_x86_opcode_nop;
			byte* call_address;

			call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK));
			Memory::WriteRelativeCall(&DrawIndexedPrimitive_Object,
				call_address, true);
			Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));

			call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK));
			Memory::WriteRelativeCall(&DrawIndexedPrimitive_Structure,
				call_address, true);
			Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));
		}
		void		c_gbuffer_system::Dispose()
		{
			c_settings_gbuffer::Unregister(Settings::Manager());

			g_default_system.Dtor();
		}

		void		c_gbuffer_system::Update(real delta_time)
		{
			c_gbuffer_system::g_output_velocity = true;

			g_stored_wvp_index = 1 - g_stored_wvp_index;

			g_wvp_stored = false;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the GBuffers Direct3D resources. </summary>
		/// <param name="pDevice">	  	[in] The current render device. </param>
		/// <param name="pParameters">
		/// 	[in] The current presentation parameters.
		/// </param>
		///-------------------------------------------------------------------------------------------------
		void c_gbuffer_system::Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			if(CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
				return;

			g_debug_index = 0;
			g_default_system.AllocateResources(pDevice, pParameters);
		}

		void		c_gbuffer_system::OnLostDevice()
		{
			g_default_system.OnLostDeviceImpl();
		}

		void		c_gbuffer_system::OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			if(CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
				return;

			g_default_system.OnResetDeviceImpl(pParameters);
		}

		void		c_gbuffer_system::Render()	{}

		void		c_gbuffer_system::Release()
		{
			g_default_system.ReleaseResources();
		}

		bool&		c_gbuffer_system::RenderGBuffer() { return g_default_system.RenderGBufferImpl(); }

		HRESULT		c_gbuffer_system::DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
		{
			HRESULT hr = S_OK;
			// Get the current render target
			LPDIRECT3DSURFACE9 origTarget;
			pDevice->GetRenderTarget(0, &origTarget);

			LPDIRECT3DVERTEXSHADER9 origVShader;
			pDevice->GetVertexShader(&origVShader);

			pDevice->SetVertexShaderConstantF(222, g_stored_worldviewproj[g_stored_wvp_index], 4);

			// Using the transpose of the world view for normal output
			Render::s_render_frustum_matricies Matricies;
			Render::RenderGlobals()->frustum.GetMatricies(Matricies);

			D3DXMATRIX WorldViewT;
			Matricies.world_view_transpose->ConvertTo4x4(WorldViewT);
			pDevice->SetVertexShaderConstantF(226, WorldViewT, 3);

			D3DXVECTOR4 linear_depth_mul;
			linear_depth_mul.x = pow(Render::RenderGlobals()->frustum.z_far, -1.0f);
			pDevice->SetVertexShaderConstantF(229, linear_depth_mul, 1);

			// We don't want to write to the Z-Buffer as it's already been done
			DWORD ZWriteEnable;
			pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &ZWriteEnable);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			hr = g_default_system.DrawIndexedPrimitive_Impl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, ZWriteEnable);

			pDevice->SetVertexShader(origVShader);
			Yelo::safe_release(origVShader); 

			pDevice->SetRenderTarget(0, origTarget);
			Yelo::safe_release(origTarget);

			return hr;
		}

		HRESULT		c_gbuffer_system::DrawIndexedPrimitive_Structure(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
		{
			HRESULT hr = pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

			if(g_default_system.IsRenderable() &&
				!Render::IsRenderingReflection() && g_current_render_state == Enums::_render_progress_structure)
			{
				hr &= g_default_system.DrawIndexedPrimitive_StructureImpl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				hr &= DrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}

			return hr;
		}
		HRESULT		c_gbuffer_system::DrawIndexedPrimitive_Object(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			HRESULT hr = pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

			if(g_default_system.IsRenderable() &&
				!Render::IsRenderingReflection() &&
				(g_current_render_state == Enums::_render_progress_sky || g_current_render_state == Enums::_render_progress_objects))
			{
				hr &= g_default_system.DrawIndexedPrimitive_ObjectImpl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				hr &= DrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}

			return hr;
		}

		bool		c_gbuffer_system::SetViewProj(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount)
		{
			if(Render::IsRenderingReflection() == false && g_system_enabled && !g_wvp_stored)
			{
				g_stored_worldviewproj[1 - g_stored_wvp_index] = *CAST_PTR(D3DMATRIX*, (float*)pConstantData);
				g_wvp_stored = true;
			}

			return true;
		}
		void		c_gbuffer_system::ClearGBuffer(IDirect3DDevice9* pDevice)
		{
			g_default_system.ClearGBufferImpl(pDevice);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system implementation
		void		c_gbuffer_system::OnLostDeviceImpl()
		{
			if(m_gbuffer_ps) m_gbuffer_ps->OnLostDevice();
			if(m_gbuffer_vs) m_gbuffer_vs->OnLostDevice();

			GBufferClear().OnLostDevice();
			GBufferDebug().OnLostDevice();

			ReleaseResources();
		}

		void		c_gbuffer_system::OnResetDeviceImpl(D3DPRESENT_PARAMETERS* params)
		{
			AllocateResources(DX9::Direct3DDevice(), params);
			if(m_gbuffer_ps) m_gbuffer_ps->OnResetDevice();
			if(m_gbuffer_vs) m_gbuffer_vs->OnResetDevice();

			GBufferClear().OnResetDevice();
			GBufferDebug().OnResetDevice();
		}
		void		c_gbuffer_system::AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* params)
		{
			if(FAILED(pDevice->TestCooperativeLevel()))
				return;

			if(Rasterizer::RasterizerConfig()->disable_render_targets || Rasterizer::RasterizerConfig()->disable_alpha_render_targets)
				return;

			// determine the maximum pixel shader profile the graphics device supports
			struct{
				byte minor_version;
				byte major_version;
			} ps_version, vs_version;

			DX9::GetSMVersion(DX9::D3DCaps()->VertexShaderVersion, vs_version.major_version, vs_version.minor_version);
			DX9::GetSMVersion(DX9::D3DCaps()->PixelShaderVersion, ps_version.major_version, ps_version.minor_version);

			if(vs_version.major_version < 3) return;
			if(ps_version.major_version < 3) return;

			if(FAILED(LoadEffect(pDevice, &m_gbuffer_ps,		"GBuffer_PS")))		return;

			const char* gbuffer_shader_set;
			if(Rasterizer::ShaderExtension::ExtensionsEnabled())
				gbuffer_shader_set = "GBuffer_VS_OS";
			else
				gbuffer_shader_set = "GBuffer_VS";
			if(FAILED(LoadEffect(pDevice, &m_gbuffer_vs,		gbuffer_shader_set)))		return;

			if(FAILED(LoadEffect(pDevice, &GBufferClear().GetEffect(),	"GBuffer_Clear")))	return;
			if(FAILED(LoadEffect(pDevice, &GBufferDebug().GetEffect(),	"GBuffer_Debug")))	return;

			// Create the position texture
			m_gbuffer.m_rt_depth.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_R32F);
			// Create the velocity texture
			m_gbuffer.m_rt_velocity.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_G16R16F);
			// Create the normals texture
			m_gbuffer.m_rt_normals_index.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_A8R8G8B8);

			m_multi_rt.count = (DX9::D3DCaps()->NumSimultaneousRTs > k_maximum_multi_render_target ?
				k_maximum_multi_render_target : DX9::D3DCaps()->NumSimultaneousRTs);

			GBufferClear().m_multi_rt.count = m_multi_rt.count;

			// cant alpha test and put data in the alpha channel with 1 render target
			if(m_multi_rt.count == 1)
				return;

			if(FAILED(GBufferClear().AllocateResources(pDevice, params->BackBufferWidth, params->BackBufferHeight)))
				return;
			if(FAILED(GBufferDebug().AllocateResources(pDevice, params->BackBufferWidth, params->BackBufferHeight)))
				return;

			m_structures.vs_bsp_techniques.n = GetTechnique(m_gbuffer_vs, "BSP", "N");
			m_structures.vs_bsp_techniques.n_v = GetTechnique(m_gbuffer_vs, "BSP", "N_V");
			m_structures.vs_object_techniques.n = GetTechnique(m_gbuffer_vs, "Object", "N");
			m_structures.vs_object_techniques.tbn = GetTechnique(m_gbuffer_vs, "Object", "TBN");
			m_structures.vs_object_techniques.n_v = GetTechnique(m_gbuffer_vs, "Object", "N_V");
			m_structures.vs_object_techniques.tbn_v = GetTechnique(m_gbuffer_vs, "Object", "TBN_V");
			m_structures.vs_object_techniques.none = GetTechnique(m_gbuffer_vs, "Object", "NONE");

			m_structures.ps_bsp_techniques.n = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "BSP", "N");
			m_structures.ps_bsp_techniques.n_v = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "BSP", "N_V");
			m_structures.ps_object_techniques.n = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "Object", "N");
			m_structures.ps_object_techniques.tbn = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "Object", "TBN");
			m_structures.ps_object_techniques.n_v = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "Object", "N_V");
			m_structures.ps_object_techniques.tbn_v = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "Object", "TBN_V");
			m_structures.ps_object_techniques.none = GetTechnique(m_gbuffer_ps, m_multi_rt.count, "Object", "NONE");

			switch (m_multi_rt.count)
			{
			case 2:
				m_multi_rt.output[0][0] = (m_gbuffer.m_rt_depth.IsEnabled()	?			m_gbuffer.m_rt_depth.surface : nullptr);
				m_multi_rt.output[1][0] = (m_gbuffer.m_rt_velocity.IsEnabled() ?		m_gbuffer.m_rt_velocity.surface : nullptr);
				m_multi_rt.output[1][1] = (m_gbuffer.m_rt_normals_index.IsEnabled() ?	m_gbuffer.m_rt_normals_index.surface : nullptr);

				GBufferClear().m_multi_rt.output[0][0] = m_multi_rt.output[0][0];
				GBufferClear().m_multi_rt.output[1][0] = m_multi_rt.output[1][0];
				GBufferClear().m_multi_rt.output[1][1] = m_multi_rt.output[1][1];
				break;
			default:
				m_multi_rt.output[0][0] = (m_gbuffer.m_rt_depth.IsEnabled() ?			m_gbuffer.m_rt_depth.surface : nullptr);
				m_multi_rt.output[0][1] = (m_gbuffer.m_rt_velocity.IsEnabled() ?		m_gbuffer.m_rt_velocity.surface : nullptr);
				m_multi_rt.output[0][2] = (m_gbuffer.m_rt_normals_index.IsEnabled() ?	m_gbuffer.m_rt_normals_index.surface : nullptr);

				GBufferClear().m_multi_rt.output[0][0] = m_multi_rt.output[0][0];
				GBufferClear().m_multi_rt.output[0][1] = m_multi_rt.output[0][1];
				GBufferClear().m_multi_rt.output[0][2] = m_multi_rt.output[0][2];
				break;
			}

			g_current_render_state = Enums::_render_progress_none;
			m_is_loaded = true;
		}

		void		c_gbuffer_system::ReleaseResources()
		{
			m_gbuffer.ReleaseTargets();

			Yelo::safe_release(m_gbuffer_ps);
			Yelo::safe_release(m_gbuffer_vs);

			GBufferClear().ReleaseResources();
			GBufferDebug().ReleaseResources();

			m_is_loaded = false;
		}

		bool&		c_gbuffer_system::RenderGBufferImpl() { return m_render_gbuffer; }

		HRESULT		c_gbuffer_system::DrawIndexedPrimitive_Impl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
		{
			HRESULT hr = S_OK;

			UINT cPasses, p;
			m_gbuffer_vs->Begin(nullptr, 0);
			m_gbuffer_vs->BeginPass(0);

			m_gbuffer_ps->Begin(&cPasses, 0);

			for( p = 0; p < cPasses; ++p )
			{
				for(uint32 i = 0; i < m_multi_rt.count; i++)
					pDevice->SetRenderTarget(i, m_multi_rt.output[p][i]);

				m_gbuffer_ps->BeginPass(p);

				pDevice->SetPixelShaderConstantF(0, g_pixel_shader_input, 1);

				hr = pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_gbuffer_ps->EndPass();

				for(uint32 i = 1; i < m_multi_rt.count; i++)
					pDevice->SetRenderTarget(i, nullptr);
			}
			m_gbuffer_ps->End();

			m_gbuffer_vs->EndPass();
			m_gbuffer_vs->End();

			return hr;
		}
		HRESULT		c_gbuffer_system::DrawIndexedPrimitive_StructureImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
		{
			if(!m_is_loaded || !m_render_gbuffer || !g_system_enabled)
				return S_OK;

			if(c_gbuffer_system::g_output_velocity && !GameState::GameTimeGlobals()->paused)
			{
				m_gbuffer_vs->SetTechnique(m_structures.vs_bsp_techniques.n_v);
				m_gbuffer_ps->SetTechnique(m_structures.ps_bsp_techniques.n_v);
				g_pixel_shader_input.z = 1.0f;
			}
			else
			{
				m_gbuffer_vs->SetTechnique(m_structures.vs_bsp_techniques.n);
				m_gbuffer_ps->SetTechnique(m_structures.ps_bsp_techniques.n);
				g_pixel_shader_input.z = 0.0f;
			}
			g_pixel_shader_input.x = 2.0f * (1.0f / 255.0f);
			g_pixel_shader_input.y = 0.0f;

			return S_OK;
		}
		HRESULT		c_gbuffer_system::DrawIndexedPrimitive_ObjectImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
		{
			byte MeshIndex = 0;
			byte TeamIndex = 0;

			if(c_gbuffer_system::g_current_object_lod < 2)
			{
				m_gbuffer_vs->SetTechnique(m_structures.vs_object_techniques.n);
				m_gbuffer_ps->SetTechnique(m_structures.ps_object_techniques.n);
			}
			else if(g_current_render_state == Enums::_render_progress_sky)
			{
				m_gbuffer_vs->SetTechnique(m_structures.vs_object_techniques.none);
				m_gbuffer_ps->SetTechnique(m_structures.ps_object_techniques.none);
			}
			else
			{
				bool do_velocity = c_gbuffer_system::g_output_object_velocity &&
					c_gbuffer_system::g_output_velocity &&
					!GameState::GameTimeGlobals()->paused;
				if(c_gbuffer_system::g_output_object_tbn)
				{
					m_gbuffer_vs->SetTechnique(
						(do_velocity ?
							m_structures.vs_object_techniques.tbn_v :
							m_structures.vs_object_techniques.tbn));
					
					m_gbuffer_ps->SetTechnique(
						(do_velocity ?
							m_structures.ps_object_techniques.tbn_v :
							m_structures.ps_object_techniques.tbn));
				}
				else
				{
					m_gbuffer_vs->SetTechnique(
						(do_velocity ?
							m_structures.vs_object_techniques.n_v :
							m_structures.vs_object_techniques.n));
					
					m_gbuffer_ps->SetTechnique(
						(do_velocity ?
							m_structures.ps_object_techniques.n_v :
							m_structures.ps_object_techniques.n));
				}

				if(do_velocity)
				{
					real DT = GameState::MainGlobals()->delta_time;
					g_pixel_shader_input.z = 1.0f;
				}
				else
					g_pixel_shader_input.z = 0.0f;
			}

			if(g_current_render_state == Enums::_render_progress_sky)
				MeshIndex = 1;
			else if(!g_object_index.IsNull())
			{
				Objects::s_object_data* g_object = blam::object_get(g_object_index);

				TeamIndex = g_object->owner_team + 1; // add one to account for '_game_team_none'
				MeshIndex = g_object->type + 3;

				if(GameEngine::Current() != nullptr)	// becomes non-null during multiplayer
					TeamIndex += Enums::k_number_of_game_teams;	// Offset TeamIndex by game_teams for MP teams

				if(g_object->VerifyType(Enums::_object_type_mask_unit))
				{
					Players::s_player_datum* player = Players::LocalPlayer();
					Objects::s_object_data* player_object = blam::object_get(player->slave_unit_index);

					if(blam::game_team_is_enemy(player_object->owner_team, g_object->owner_team))
						TeamIndex |= 1 << 5;

					if(g_object->damage.health <= 0.0f)
						TeamIndex |= 1 << 6;
				}
			}

			g_pixel_shader_input.x = (float)MeshIndex * (1.0f / 255.0f);
			g_pixel_shader_input.y = (float)TeamIndex * (1.0f / 255.0f);

			return S_OK;
		}

		void		c_gbuffer_system::ClearGBufferImpl(IDirect3DDevice9* pDevice)
		{
			if(!g_system_enabled)
				return;

			// Get the current render target
			LPDIRECT3DSURFACE9 origTarget;
			pDevice->GetRenderTarget(0, &origTarget);

			// Clear our outputs
			GBufferClear().Render(pDevice);

			// Set render target back to original
			pDevice->SetRenderTarget(0, origTarget);
			Yelo::safe_release(origTarget);
		}

		void		c_gbuffer_system::RenderDebugQuad(IDirect3DDevice9* pDevice)
		{
			if(!g_system_enabled || !m_is_loaded)
			{
				g_debug_index = 0;
				return;
			}
			GBufferDebug().Render(pDevice, g_debug_index);
		}

		HRESULT		c_gbuffer_system::LoadEffect(IDirect3DDevice9* pDevice, LPD3DXEFFECT* pEffect, const char* EffectID)
		{
			HRESULT hr = S_OK;
			LPD3DXBUFFER error_buffer = nullptr;
			uint32 data_size = 0;
			void* data_pointer = m_shader_package.GetDataPointer(EffectID, &data_size);

			if(!data_pointer || data_size == 0)
				return E_FAIL;

			hr = D3DXCreateEffect(
				pDevice,
				data_pointer,
				data_size,
				nullptr,
				nullptr,
				0,
				nullptr,
				pEffect,
				&error_buffer
			);
			if (FAILED(hr))
			{
				// if loading the shader failed, and we are in devmode
				// print an error to the console
				if(GameState::DevmodeEnabled())
				{
					Rasterizer::PostProcessing::ErrorReporting::Write("Error: failed to load gbuffer shaders");
					Rasterizer::PostProcessing::ErrorReporting::WriteD3DXErrors(error_buffer);
				}
			}
			Yelo::safe_release(error_buffer);

			return hr;
		}
		c_gbuffer&	c_gbuffer_system::GBuffer() { return c_gbuffer_system::g_default_system.m_gbuffer; }
		/////////////////////////////////////////////////////////////////////
		// c_gbuffer_system		
		datum_index				c_gbuffer_system::g_object_index;
		c_gbuffer_system		c_gbuffer_system::g_default_system;
		int16					c_gbuffer_system::g_debug_index;
		bool					c_gbuffer_system::g_system_enabled;
		Enums::render_progress	c_gbuffer_system::g_current_render_state;
		uint32					c_gbuffer_system::g_current_object_lod;
		BOOL					c_gbuffer_system::g_output_object_tbn;
		BOOL					c_gbuffer_system::g_output_object_velocity;
		BOOL					c_gbuffer_system::g_output_velocity;
		D3DXVECTOR4				c_gbuffer_system::g_pixel_shader_input;

		D3DXMATRIX				c_gbuffer_system::g_identity_matrix;
		BOOL					c_gbuffer_system::g_wvp_stored;
		D3DXMATRIX				c_gbuffer_system::g_stored_worldviewproj[2];
		BOOL					c_gbuffer_system::g_stored_wvp_index;

		BOOL&		c_gbuffer_system::OutputObjectTBN() { return c_gbuffer_system::g_output_object_tbn; }

		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system::render_progress
		void		c_gbuffer_system::render_progress::SkyPreProcess()
		{
			g_current_render_state = Enums::_render_progress_sky;
		}
		void		c_gbuffer_system::render_progress::SkyPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::ObjectsPreProcess()
		{
			g_current_render_state = Enums::_render_progress_objects;
		}
		void		c_gbuffer_system::render_progress::ObjectsPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::StructurePreProcess()
		{
			g_current_render_state = Enums::_render_progress_structure;
		}
		void		c_gbuffer_system::render_progress::StructurePostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::UIPreProcess()
		{
			if(g_debug_index != 0)
				g_default_system.RenderDebugQuad(DX9::Direct3DDevice());
		}
		//////////////////////////////////////////////////////////////////////
	};
};
#endif
