/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DeviceHooks.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/main/main_yelo_base.hpp>

#include "Memory/MemoryInterface.hpp"

#include "Common/GameSystems.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Interface/GameUI.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace DeviceHooks
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_DEVICEHOOKS
#include "Memory/_EngineLayout.inl"

		static bool g_corruption_hack_enabled = false;
		static real g_corruption_hack_null_data[12264];

		static void CreateD3DHook(void* call_address, void* hook_address)
		{
			Memory::WriteRelativeCall(hook_address, call_address, true);
			byte* nop_address = CAST_PTR(byte*, call_address) + 5;
			*nop_address = Enums::_x86_opcode_nop;
		}

		static HRESULT CreateDevice(IDirect3D9* d3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface)
		{
			HRESULT hr = d3d->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

			if(SUCCEEDED(hr) && Yelo::Main::IsYeloEnabled())
			{
				// If the device vendor is ATI, enabled the corruption hack fix
				D3DADAPTER_IDENTIFIER9 device_identifier;
				if(SUCCEEDED(d3d->GetAdapterIdentifier(0, 0, &device_identifier)))
				{
					g_corruption_hack_enabled = device_identifier.VendorId == 0x1002;
				}

				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].Initialize(*ppReturnedDeviceInterface, pPresentationParameters);
			}

			return hr;
		}

		static HRESULT Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS *pPresentationParameters)
		{
			if(Yelo::Main::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].OnLostDevice();
			}

			HRESULT hr = device->Reset(pPresentationParameters);

			if(Yelo::Main::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].OnResetDevice(pPresentationParameters);
			}

			return hr;
		}

		static HRESULT BeginScene(IDirect3DDevice9* device)
		{
			HRESULT hr = device->BeginScene();

			if(Yelo::Main::IsYeloEnabled())
				Yelo::DX9::c_gbuffer_system::ClearGBuffer(device);

			return hr;
		}

		static HRESULT EndScene(IDirect3DDevice9* device)
		{
			if(Yelo::Main::IsYeloEnabled())
			{
				if(g_corruption_hack_enabled)
				{
					DX9::Direct3DDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, 0x1FF, &g_corruption_hack_null_data, 0x18);
				}

				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].Render();
			}

			return device->EndScene();
		}

		static HRESULT SetVertexShaderConstantF_ViewProj(IDirect3DDevice9* device, UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
		{
			DX9::c_gbuffer_system::SetViewProj(device, pConstantData, Vector4fCount);
			Rasterizer::ShaderExtension::Model::SetViewProj(device, pConstantData, Vector4fCount);

			return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}

		static HRESULT SetVertexShaderConstantF_ModelSpecColor(IDirect3DDevice9* device, UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
		{
			bool set_original = false;

			set_original |= Rasterizer::ShaderExtension::Model::SetSpecColor(device, pConstantData, Vector4fCount);

			if(set_original)
				return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
			return S_OK;
		}

		static HRESULT SetVertexShaderConstantF_ModelVertexLight(IDirect3DDevice9* device, UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
		{
			bool set_original = false;

			set_original |= Rasterizer::ShaderExtension::Model::SetVertexLight(device, pConstantData, Vector4fCount);

			if(set_original)
				return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
			return S_OK;
		}

		__declspec(naked) static void Hook_D3DCreateDeviceCall()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_D3D_CREATE_DEVICE_RETN);

			_asm {
				push	ebp
				push	eax

				call	DeviceHooks::CreateDevice
				jmp		RETN_ADDRESS
			}
		}

		__declspec(naked) static void Hook_D3DResetCall()
		{
			API_FUNC_NAKED_START_()
				push	esi
				push	eax
				call	DeviceHooks::Reset
			API_FUNC_NAKED_END_()
		}

		void Initialize()
		{
			ZeroMemory(&g_corruption_hack_null_data[0], sizeof(g_corruption_hack_null_data));

			Memory::WriteRelativeJmp(Hook_D3DCreateDeviceCall, GET_FUNC_VPTR(RASTERIZER_D3D_CREATE_DEVICE_HOOK), true);
			Memory::WriteRelativeCall(Hook_D3DResetCall, GET_FUNC_VPTR(RASTERIZER_D3D_RESET_DEVICE_HOOK), true);

			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_D3D_BEGIN_SCENE_CALL), BeginScene);
			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_D3D_END_SCENE_CALL), EndScene);

			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL), SetVertexShaderConstantF_ViewProj);
			
			for (auto call_address : K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALLS)
				CreateD3DHook(call_address, SetVertexShaderConstantF_ModelSpecColor);

			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL), SetVertexShaderConstantF_ModelVertexLight);
		}

		void Dispose()
		{
		}
	};};
};
#endif
