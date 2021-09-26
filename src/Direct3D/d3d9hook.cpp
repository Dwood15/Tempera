/*
	Project: tempera
	File: d3d9hook.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#define WIN32_LEAN_AND_MEAN

#include <addlog.h>
#include <enums/player_enums.h>
#include "d3d9hook.h"
#include "textures.h"
#include "../gamestate/camera.h"
#include "../RuntimeManager.h"
#include "../gamestate/objects/objectcontroller.h"

CD3D::CD3D() {
	Font = NULL;
}

CD3D::~CD3D() {
}

long CD3D::GenerateShader(IDirect3DDevice9 *pDevice, IDirect3DPixelShader9 **pShader, float r, float g, float b) {
	char        szShader[256];
	ID3DXBuffer *pShaderBuf = NULL;
	sprintf(szShader, "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", r, g, b, 1.0f);
	D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuf, NULL);
	if (FAILED(pDevice->CreatePixelShader((const DWORD *) pShaderBuf->GetBufferPointer(), pShader)))
		return E_FAIL;
	return S_OK;
}

struct tri_vert {
	float    x, y, z;
	D3DCOLOR colour;
};

void CD3D::myDrawText(IDirect3DDevice9 *pDevice, bool shadow, long x, long y, long width, long height, D3DCOLOR color, D3DCOLOR shadowcolor, const char *cFmt, ...) {
	va_list mvalist;
	char    cBuffer[256] = {0};

	RECT Rect1 = {x + 1, y, x + width, y + height};
	RECT Rect2 = {x - 1, y, x + width, y + height};
	RECT Rect3 = {x, y + 1, x + width, y + height};
	RECT Rect4 = {x, y - 1, x + width, y + height};
	RECT Rect5 = {x, y, x + width, y + height};

		va_start(mvalist, cFmt);
	_vsnprintf(cBuffer, sizeof(cBuffer), cFmt, mvalist);
		va_end(mvalist);


	if (shadow) {
		Font->DrawText(NULL, cBuffer, -1, &Rect1, 0, shadowcolor);
		Font->DrawText(NULL, cBuffer, -1, &Rect2, 0, shadowcolor);
		Font->DrawText(NULL, cBuffer, -1, &Rect3, 0, shadowcolor);
		Font->DrawText(NULL, cBuffer, -1, &Rect4, 0, shadowcolor);
	}

	Font->DrawText(NULL, cBuffer, -1, &Rect5, 0, color);
}

void CD3D::FillRGBA(ID3DXLine *pLine, float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 vLine[2];
	pLine->SetWidth(width);
	pLine->SetAntialias(0);
	pLine->SetGLLines(1);
	vLine[0].x = x + width / 2;
	vLine[0].y = y;
	vLine[1].x = x + width / 2;
	vLine[1].y = y + height;
	pLine->Begin();
	pLine->Draw(vLine, 2, color);
	pLine->End();
}

void CD3D::DrawMouse(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pCursorTexture, ID3DXSprite *pCursorSprite) {
	POINT    pPoint;
	D3DCOLOR dColor = (D3DCOLOR)NULL;
	GetCursorPos(&pPoint);
	D3DXVECTOR3 Pos1;
	// Pos1[0] = x;
	// Pos1[1] = y;
	Pos1.x = (float) pPoint.x;
	Pos1.y = (float) pPoint.y;
	Pos1.z = 0.0f;
	pCursorSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pCursorSprite->Draw(pCursorTexture, NULL, NULL, &Pos1, 0xFFFFFFFF);
	pCursorSprite->End();
}

typedef long (__stdcall *pEndScene)(IDirect3DDevice9 *pDevice);

pEndScene oEndScene;

bool isOffScreen(vect3 screenpos) {
	return ((screenpos.x < (-FOV_XBOUND)) || screenpos.x > (1 + FOV_XBOUND) || (screenpos.y < (-FOV_YBOUND)) || screenpos.y > (1 + FOV_YBOUND) || screenpos.z > OBJECT_CLIP);
}

void PrintObjectTags(IDirect3DDevice9 *pDevice) {
	if (!::feature_management::engines::IsCoreInitialized()) {
		static bool printOnce = true;
		if(printOnce) {
			::PrintLn<true>("Tried to get Data for use in PrintObjects, couldn't.");
			printOnce = false;
		}
		return;
	}

	static short maxObjects = CurrentRuntime->GetMaxObjects();

	object_data   *obj          = NULL;
	object_header *objh         = NULL;
	object_header *temp_nearest = NULL;
	//CObjectArray* objA;
	//vect ObjCoord;
	const char    *ObjName;
	vect3         screenpos;

	// for choosing the 'selected' object: (or the one closest to where we're aiming.
	float closest_angle = .35f; // it needs to be at least within 30 degrees
	float tx;
	float ty;


	for (unsigned short i = 0; i < maxObjects; i++) {
		objh = CurrentRuntime->GetObjectHeader(i);
		obj  = CurrentRuntime->GetGenericObject(i);
		if (obj == NULL) {
			continue;
		}

		screenpos = CurrentRuntime->MyCamera->ScreenPos(obj->World);

		// Offscreen check
		if (isOffScreen(screenpos)) {
			continue;
		}

		tx = screenpos.x * 2 - 1;
		ty = screenpos.y * 2 - 1;
		tx = sqrt(tx * tx + ty * ty);

		//screenpos.z is also known as dist_to_obj
		if (tx < closest_angle) {
			closest_angle = tx;
			temp_nearest  = objh;
		}

		screenpos.x *= cd3d.pViewport.Width;
		screenpos.y *= cd3d.pViewport.Height;

		D3DCOLOR color = tGreen;

		ObjName = CurrentRuntime->GetObjectName(i);


		if (obj->IsPlayer()) {
			color = tLightBlue;

		}

		if (CurrentRuntime->ObjectControl->IsSelected(objh)) {
			color = tOrange;

		} else if (CurrentRuntime->ObjectControl->IsNearest(objh)) {
			color = tBlue;
		}

		cd3d.myDrawText(pDevice, true, (int) screenpos.x, (int) screenpos.y, 1000, 1000, color, tBlack, ObjName);
	}

	CurrentRuntime->ObjectControl->SetNearest(temp_nearest);
}

long __stdcall hkEndScene(IDirect3DDevice9 *pDevice) {
	if (!cd3d.Font) {
		D3DXCreateFont(pDevice, 15, 0, FW_BOLD, 1, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Ariel", &cd3d.Font);
	}

	pDevice->GetViewport(&cd3d.pViewport);

	PrintObjectTags(pDevice);

	return oEndScene(pDevice);
}

typedef long (__stdcall *pReset)(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);

pReset oReset;

long __stdcall hkReset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters) {

	if (cd3d.Font)
		cd3d.Font->OnLostDevice();

	HRESULT hResult = oReset(pDevice, pPresentationParameters);

	if (cd3d.Font)
		cd3d.Font->OnResetDevice();

	return hResult;
}

typedef long (__stdcall *pDrawIndexedPrimitive)(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, int BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

pDrawIndexedPrimitive oDrawIndexedPrimitive;

long __stdcall hkDrawIndexedPrimitive(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
#ifndef __GNUC__
	__asm pushad

	__asm popad

	return oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
#else
	return 0;
#endif
}

typedef long (__stdcall *pSetStreamSource)(IDirect3DDevice9 *pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride);

pSetStreamSource oSetStreamSource;

long __stdcall hkSetStreamSource(IDirect3DDevice9 *pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride) {
#ifndef __GNUC__
	__asm pushad

	__asm popad

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
#else
	return 0;
#endif
}

// This function grabs the device pointer from Halo
// Then I assign the pointer to the vTable structure
// Then you can use DetourFunction to detour any of the functions
//    in the vTable

// vTable is just a huge table of pointers to every d3d9 function
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wendif-labels"
#endif
#include <detours.h>

DWORD __stdcall CD3D::hkD3DHook(void *lpVoid) {
	vTable_D3D9 *vD3D9; // Create instance of d3d9 Virtual Method Table

	//1.08 d3d device global 0x0071D09C
	//0x3C471C0
	//0x3C49F5C
	//1.10 global.. 0x6B840C

	void             *pDevicePointer = (void *) 0x6B840C; // Halo 1.10 version device pointer ( static )
	DWORD            dwOldProtect    = (DWORD)NULL;
	IDirect3DDevice9 *pGameDevice;

	//VirtualProtect((void *) pDevicePointer, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(&pGameDevice, (void *) pDevicePointer, 4);
	//VirtualProtect((void *) pDevicePointer, 4, dwOldProtect, NULL);

	DWORD *pHaloDevice = (DWORD *) pGameDevice;
	pHaloDevice = (DWORD *) pHaloDevice[0];
	vD3D9       = (vTable_D3D9 *) pHaloDevice; // Assign device table to our vTable structure

	oEndScene = (pEndScene) DetourFunction((PBYTE) vD3D9->EndScene, (PBYTE) hkEndScene);
	oReset    = (pReset) DetourFunction((PBYTE) vD3D9->Reset, (PBYTE) hkReset);
	//oDrawIndexedPrimitive = (pDrawIndexedPrimitive)DetourFunction((PBYTE)vD3D9->DrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
	//oSetStreamSource = (pSetStreamSource)DetourFunction((PBYTE)vD3D9->SetStreamSource, (PBYTE)hkSetStreamSource);

	return 0;
}
