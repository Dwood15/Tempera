/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace ErrorReporting
	{
		// TODO: these functions should be outside of the postprocessing system,
		// as other code may want the functionality they provide
		enum
		{
			k_max_error_line_count = 3,
		};

		void Write(cstring string);
		void WriteLine(cstring format, ...);
		void WriteD3DXErrors(LPD3DXBUFFER buffer);
	};};};
};
#endif