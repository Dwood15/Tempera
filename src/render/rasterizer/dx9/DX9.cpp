/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DX9.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/cseries/pc_crashreport.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_DX9
#include "Memory/_EngineLayout.inl"

		static IDirect3D9* d3d;
		static IDirect3DDevice9* global_d3d_device;

		void Initialize()
		{
#if !defined(DX_WRAPPER)
			while(!GET_DPTR(D3D) && !GET_DPTR(Device)) Sleep(100);

			d3d = GET_DPTR(D3D);
			global_d3d_device = GET_DPTR(Device);

			//	if(!d3d)
			//		MessageBox(NULL, "Failure: d3d", "debug", MB_OK);
			//	if(!global_d3d_device)
			//		MessageBox(NULL, "Failure: d3d device", "debug", MB_OK);
#endif
		}

		void Dispose() {}

#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
		{
			d3d = GET_DPTR(D3D);
			global_d3d_device = GET_DPTR(Device);

			D3DADAPTER_IDENTIFIER9 d3d_id;
			if(SUCCEEDED(d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &d3d_id)))
			{
				char property_string[256] = "";

				Debug::AddPropertyToCrashReport("D3DDriver", d3d_id.Driver);
				Debug::AddPropertyToCrashReport("D3DDescription", d3d_id.Description);
				Debug::AddPropertyToCrashReport("D3DDeviceName", d3d_id.DeviceName);

				if(-1 != sprintf_s(property_string, sizeof(property_string), "%i.%i.%i.%i",
					HIWORD(d3d_id.DriverVersion.HighPart),
					LOWORD(d3d_id.DriverVersion.HighPart),
					HIWORD(d3d_id.DriverVersion.LowPart),
					LOWORD(d3d_id.DriverVersion.LowPart)))
				{
					Debug::AddPropertyToCrashReport("D3DDriverVersion", property_string);
				}

				if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.VendorId))
					Debug::AddPropertyToCrashReport("D3DVendorId", property_string);

				if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.DeviceId))
					Debug::AddPropertyToCrashReport("D3DDeviceId", property_string);

				if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.SubSysId))
					Debug::AddPropertyToCrashReport("D3DSubSysId", property_string);

				if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.Revision))
					Debug::AddPropertyToCrashReport("D3DRevision", property_string);
			}
		}

		void OnLostDevice()									{}
		void OnResetDevice(D3DPRESENT_PARAMETERS* params)	{}
		void Render()										{}
		void Release()										{}
#endif

		D3DPRESENT_PARAMETERS*	D3DPresetParams()		PTR_IMP_GET2(Params);
		IDirect3D9*				Direct3D9()				{ return d3d; }
		D3DCAPS9*				D3DCaps()				PTR_IMP_GET2(Caps);
		IDirect3DDevice9*		Direct3DDevice()		{ return global_d3d_device; }
		IDirectInput8*			DirectInput8()			DPTR_IMP_GET(DInput8);
		IDirectInputDevice8*	InputDeviceKeyboard()	DPTR_IMP_GET(DInput8DeviceKeyboard);
		IDirectInputDevice8*	InputDeviceMouse()		DPTR_IMP_GET(DInput8DeviceMouse);
		IDirectInputDevice8*	InputDeviceJoystick(short index)
		{
			if(index >= 8) return NULL;
			return GET_DPTR2(DInput8DeviceJoysticks)[index];
		}

		void GetSMVersion(const DWORD sm_version, byte& major, byte& minor)
		{
			union{
				DWORD version;
				struct{
					struct{
						char minor_version;
						char major_version;
					};
					WORD type;
				};
			};

			version = sm_version;
			major = major_version;
			minor = minor_version;
		}
	};
};
#endif