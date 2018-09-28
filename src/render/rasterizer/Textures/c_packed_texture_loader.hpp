/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	class c_packed_file;

	namespace Rasterizer { namespace Textures
	{
		/// <summary>	Loader for packed textures. </summary>
		class c_packed_texture_loader final
		{
			c_packed_file& m_packed_file;

		public:
			/// <summary>	Default constructor. </summary>
			c_packed_texture_loader(c_packed_file& packed_file);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Loads a texture. </summary>
			///
			/// <param name="texture_id">	 	Identifier for the texture. </param>
			/// <param name="target_texture">	[out] The target texture pointer. </param>
			/// <param name="image_info">	 	[out] If non-null, information describing the image. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool LoadTexture(const std::string& texture_id, IDirect3DTexture9** target_texture, D3DXIMAGE_INFO* image_info = nullptr);
		};
	};};
};
#endif