#include <d3d9.h>
#include <stdio.h>

#include <InitGuid.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#undef DIRECTINPUT_VERSION

#include "../../../memory/memory_interface_base.hpp"

namespace Yelo::DX9 {
	static IDirect3D9       *d3d;
	static IDirect3DDevice9 *global_d3d_device;

	void Initialize() {

	}

	void Dispose() {}

	void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
	{
		d3d = *reinterpret_cast<IDirect3D9 **>(0x6B8410);;
		global_d3d_device = *reinterpret_cast<IDirect3DDevice9 **>(0x6B840C);;

		D3DADAPTER_IDENTIFIER9 d3d_id;
		if(SUCCEEDED(d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &d3d_id)))
		{
			char property_string[256] = "";

			// Debug::AddPropertyToCrashReport("D3DDriver", d3d_id.Driver);
			// Debug::AddPropertyToCrashReport("D3DDescription", d3d_id.Description);
			// Debug::AddPropertyToCrashReport("D3DDeviceName", d3d_id.DeviceName);

			if(-1 != sprintf_s(property_string, sizeof(property_string), "%i.%i.%i.%i",
				HIWORD(d3d_id.DriverVersion.HighPart),
				LOWORD(d3d_id.DriverVersion.HighPart),
				HIWORD(d3d_id.DriverVersion.LowPart),
				LOWORD(d3d_id.DriverVersion.LowPart)))
			{
				// Debug::AddPropertyToCrashReport("D3DDriverVersion", property_string);
			}

			// if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.VendorId))
			// 	Debug::AddPropertyToCrashReport("D3DVendorId", property_string);
			//
			// if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.DeviceId))
			// 	Debug::AddPropertyToCrashReport("D3DDeviceId", property_string);
			//
			// if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.SubSysId))
			// 	Debug::AddPropertyToCrashReport("D3DSubSysId", property_string);
			//
			// if(-1 != sprintf_s(property_string, sizeof(property_string), "0x%X", d3d_id.Revision))
			// 	Debug::AddPropertyToCrashReport("D3DRevision", property_string);
		}
	}

	void OnLostDevice()									{}
	void OnResetDevice(D3DPRESENT_PARAMETERS* params)	{}
	void Render()										{}
	void Release()										{}

	D3DPRESENT_PARAMETERS *D3DPresetParams() { return reinterpret_cast<D3DPRESENT_PARAMETERS *>(0x75B760); }

	IDirect3D9 *Direct3D9() { return d3d; }

	D3DCAPS9 *D3DCaps() { return reinterpret_cast<D3DCAPS9 *>(0x75C420); }

	IDirect3DDevice9 *Direct3DDevice() { return global_d3d_device; }

	IDirectInput8 *DirectInput8() { return *reinterpret_cast<IDirectInput8 **>(0x64C52C); }

	IDirectInputDevice8 *InputDeviceKeyboard() { return *reinterpret_cast<IDirectInputDevice8 **>(0x64C730); }

	IDirectInputDevice8 *InputDeviceMouse() { reinterpret_cast<IDirectInputDevice8 **>(0x64C734); }

	IDirectInputDevice8 *InputDeviceJoystick(short index) {
		if (index >= 8) {
			return nullptr;
		}
		return reinterpret_cast<IDirectInputDevice8 **>(0x64C778)[index];
	}

	void GetSMVersion(const DWORD sm_version, byte &major, byte &minor) {
		static union {
			DWORD version;
			struct {
				struct {
					char minor_version;
					char major_version;
				};
				WORD type;
			};
		} m;

		m.version = sm_version;
		major   = m.major_version;
		minor   = m.minor_version;
	}
};
#endif
