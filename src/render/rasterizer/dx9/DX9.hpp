/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace DX9
	{
		void Initialize();
		void Dispose();

#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
		void OnLostDevice();
		void OnResetDevice(D3DPRESENT_PARAMETERS* params);
		void Render();
		void Release();
#endif

		// D3D Present Properties
		D3DPRESENT_PARAMETERS* D3DPresetParams();

		// D3D Properties
		IDirect3D9* Direct3D9();

		// D3D Caps
		D3DCAPS9* D3DCaps();

		// D3D Device Properties
		IDirect3DDevice9* Direct3DDevice();

		// DInput* Properties
		IDirectInput8* DirectInput8();

		// IDirectInputDevice8 Properties
		IDirectInputDevice8* InputDeviceKeyboard();
		IDirectInputDevice8* InputDeviceMouse();
		IDirectInputDevice8* InputDeviceJoystick(int16 index);

		void GetSMVersion(const DWORD sm_version, byte& major, byte& minor);
	};
};

struct COLOR_VERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	D3DCOLOR color;

	API_INLINE void Set(float _x, float _y, float _z, float _rhw, D3DCOLOR _color)
	{
		x = _x;
		y = _y;
		z = _z;
		rhw = _rhw;
		color = _color;
	}

	API_INLINE void Set(float _x, float _y, float _z, float _rhw, float _a, float _r, float _g, float _b)
	{
		x = _x;
		y = _y;
		z = _z;
		rhw = _rhw;
		color = D3DXCOLOR(_r, _g, _b, _a);
	}
};
#define NEW_COLOR_VERTEX(vert, x, y, z, rhw, color) vert.Set((float)x, (float)y, (float)z, (float)rhw, color)

struct TEXTURE_VERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	float u;
	float v;

	API_INLINE void Set(float _x, float _y, float _z, float _rhw, float _u, float _v)
	{
		x = _x;
		y = _y;
		z = _z;
		rhw = _rhw;
		u = _u;
		v = _v;
	}
};
#define NEW_TEXTURE_VERTEX(vert, x, y, z, rhw, u, v) vert.Set((float)x, (float)y, (float)z, (float)rhw, (float)u, (float)v)

#endif