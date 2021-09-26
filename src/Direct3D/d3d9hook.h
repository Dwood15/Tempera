/*
	Project: tempera
	File: d3d9hook.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef D3D9HOOK_H
#define D3D9HOOK_H

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpadded"
#endif

#include <stdio.h>
#include <d3d9.h>


#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wpadded"
#endif
//#pragma clang diagnostic pop
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define tRed      D3DCOLOR_ARGB( 255, 255,  50,  50 )
#define tGreen      D3DCOLOR_ARGB( 255,   0, 255,   0 )
#define tBlue      D3DCOLOR_ARGB( 255,  50,  50, 255 )
#define tLightBlue   D3DCOLOR_ARGB( 255,   0, 153, 255 )
#define tYellow      D3DCOLOR_ARGB( 255, 255, 255,   0 )
#define tPurple      D3DCOLOR_ARGB( 255, 102,   0, 153 )
#define tWhite      D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define tBlack      D3DCOLOR_ARGB( 255,   0,   0,   0 )
#define tOrange      D3DCOLOR_ARGB( 255, 255, 127,   0 )
#define tPink      D3DCOLOR_ARGB( 255, 246,  90, 181 )

class vTable_D3D9 {
public:
	DWORD QueryInterface;            //0
	DWORD AddRef;                  //1
	DWORD Release;                  //2
	DWORD TestCooperativeLevel;         //3
	DWORD GetAvailableTextureMem;      //4
	DWORD EvictManagedResources;      //5
	DWORD GetDirect3D;               //6
	DWORD GetDeviceCaps;            //7
	DWORD GetDisplayMode;            //8
	DWORD GetCreationParameters;      //9
	DWORD SetCursorProperties;         //10
	DWORD SetCursorPosition;         //11
	DWORD ShowCursor;               //12
	DWORD CreateAdditionalSwapChain;   //13
	DWORD GetSwapChain;               //14
	DWORD GetNumberOfSwapChains;      //15
	DWORD Reset;                  //16
	DWORD Present;                  //17
	DWORD GetBackBuffer;            //18
	DWORD GetRasterStatus;            //19
	DWORD SetDialogBoxMode;            //20
	DWORD SetGammaRamp;               //21
	DWORD GetGammaRamp;               //22
	DWORD CreateTexture;            //23
	DWORD CreateVolumeTexture;         //24
	DWORD CreateCubeTexture;         //25
	DWORD CreateVertexBuffer;         //26
	DWORD CreateIndexBuffer;         //27
	DWORD CreateRenderTarget;         //28
	DWORD CreateDepthStencilSurface;   //29
	DWORD UpdateSurface;            //30
	DWORD UpdateTexture;            //31
	DWORD GetRenderTargetData;         //32
	DWORD GetFrontBufferData;         //33
	DWORD StretchRect;               //34
	DWORD ColorFill;               //35
	DWORD CreateOffscreenPlainSurface;   //36
	DWORD SetRenderTarget;            //37
	DWORD GetRenderTarget;            //38
	DWORD SetDepthStencilSurface;      //39
	DWORD GetDepthStencilSurface;      //40
	DWORD BeginScene;               //41
	DWORD EndScene;                  //42
	DWORD Clear;                  //43
	DWORD SetTransform;               //44
	DWORD GetTransform;               //45
	DWORD MultiplyTransform;         //46
	DWORD SetViewport;               //47
	DWORD GetViewport;               //48
	DWORD SetMaterial;               //49
	DWORD GetMaterial;               //50
	DWORD SetLight;                  //51
	DWORD GetLight;                  //52
	DWORD LightEnable;               //53
	DWORD GetLightEnable;            //54
	DWORD SetClipPlane;               //55
	DWORD GetClipPlane;               //56
	DWORD SetRenderState;            //57
	DWORD GetRenderState;            //58
	DWORD CreateStateBlock;            //59
	DWORD BeginStateBlock;            //60
	DWORD EndStateBlock;            //61
	DWORD SetClipStatus;            //62
	DWORD GetClipStatus;            //63
	DWORD GetTexture;               //64
	DWORD SetTexture;               //65
	DWORD GetTextureStageState;         //66
	DWORD SetTextureStageState;         //67
	DWORD GetSamplerState;            //68
	DWORD SetSamplerState;            //69
	DWORD ValidateDevice;            //70
	DWORD SetPaletteEntries;         //71
	DWORD GetPaletteEntries;         //72
	DWORD SetCurrentTexturePalette;      //73
	DWORD GetCurrentTexturePalette;      //74
	DWORD SetScissorRect;            //75
	DWORD GetScissorRect;            //76
	DWORD SetSoftwareVertexProcessing;   //77
	DWORD GetSoftwareVertexProcessing;   //78
	DWORD SetNPatchMode;            //79
	DWORD GetNPatchMode;            //80
	DWORD DrawPrimitive;            //81
	DWORD DrawIndexedPrimitive;         //82
	DWORD DrawPrimitiveUP;            //83
	DWORD DrawIndexedPrimitiveUP;      //84
	DWORD ProcessVertices;            //85
	DWORD CreateVertexDeclaration;      //86
	DWORD SetVertexDeclaration;         //87
	DWORD GetVertexDeclaration;         //88
	DWORD SetFVF;                  //89
	DWORD GetFVF;                  //90
	DWORD CreateVertexShader;         //91
	DWORD SetVertexShader;            //92
	DWORD GetVertexShader;            //93
	DWORD SetVertexShaderConstantF;      //94
	DWORD GetVertexShaderConstantF;      //95
	DWORD SetVertexShaderConstantI;      //96
	DWORD GetVertexShaderConstantI;      //97
	DWORD SetVertexShaderConstantB;      //98
	DWORD GetVertexShaderConstantB;      //99
	DWORD SetStreamSource;            //100
	DWORD GetStreamSource;            //101
	DWORD SetStreamSourceFreq;         //102
	DWORD GetStreamSourceFreq;         //103
	DWORD SetIndices;               //104
	DWORD GetIndices;               //105
	DWORD CreatePixelShader;         //106
	DWORD SetPixelShader;            //107
	DWORD GetPixelShader;            //108
	DWORD SetPixelShaderConstantF;      //109
	DWORD GetPixelShaderConstantF;      //110
	DWORD SetPixelShaderConstantI;      //111
	DWORD GetPixelShaderConstantI;      //112
	DWORD SetPixelShaderConstantB;      //113
	DWORD GetPixelShaderConstantB;      //114
	DWORD DrawRectPatch;            //115
	DWORD DrawTriPatch;               //116
	DWORD DeletePatch;               //117
	DWORD CreateQuery;               //118
};

#include <d3dx9.h>

class CD3D {
public:
	CD3D();

	~CD3D();

	ID3DXFont         *Font;
	IDirect3DTexture9 *CursorTex;
	ID3DXSprite       *CursorSprite;
	ID3DXLine         *Line;
	D3DVIEWPORT9      pViewport;

	long GenerateShader(IDirect3DDevice9 *pDevice, IDirect3DPixelShader9 **pShader, float r, float g, float b);

	void FillRGBA(ID3DXLine *pLine, float x, float y, float width, float height, D3DCOLOR color);

	void DrawMouse(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pCursorTexture, ID3DXSprite *pCursorSprite);

	void myDrawText(IDirect3DDevice9 *pDevice, bool shadow, long x, long y, long width, long height, D3DCOLOR color, D3DCOLOR shadowcolor, const char *cFmt, ...);
	void myDrawPlane(IDirect3DDevice9 *pDevice);

	DWORD __stdcall hkD3DHook(void *lpVoid);
};

#endif /* D3D9HOOK_H */

