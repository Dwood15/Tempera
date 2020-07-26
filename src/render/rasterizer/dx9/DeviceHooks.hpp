/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Rasterizer { namespace DeviceHooks
	{
		void Initialize();
		void Dispose();
	};};
};
#endif