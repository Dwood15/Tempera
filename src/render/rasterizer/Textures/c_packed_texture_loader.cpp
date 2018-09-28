/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Textures/c_packed_texture_loader.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Rasterizer/DX9/DX9.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace Textures
	{
#pragma region c_packed_texture_loader
		c_packed_texture_loader::c_packed_texture_loader(c_packed_file& packed_file)
			: m_packed_file(packed_file)
		{ }

		bool c_packed_texture_loader::LoadTexture(const std::string& texture_id, IDirect3DTexture9** target_texture, D3DXIMAGE_INFO* image_info)
		{
			uint32 data_size = 0;
			auto* texture_file = m_packed_file.GetDataPointer(texture_id.c_str(), &data_size);

			if(!texture_file)
			{
				target_texture = nullptr;
				return false;
			}

			IDirect3DTexture9* created_texture = nullptr;
			return S_OK == D3DXCreateTextureFromFileInMemoryEx(DX9::Direct3DDevice()
				, texture_file
				, data_size
				, 0
				, 0
				, D3DX_DEFAULT
				, 0
				, D3DFMT_UNKNOWN
				, D3DPOOL_MANAGED
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				, 0
				, image_info
				, NULL
				, target_texture);
		}
#pragma endregion
	};};
};
#endif