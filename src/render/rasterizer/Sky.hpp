/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Render { namespace Sky
	{
		void Initialize();
		void Dispose();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void Reset();
		void SetSkyIndex(const byte sky_index, const datum_index override_index);
	};};
};
#endif