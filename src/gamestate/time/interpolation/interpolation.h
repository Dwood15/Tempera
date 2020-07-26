/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Time { namespace Interpolation
	{
		template<size_t ValueCount>
		static void InterpolateValues(bool* lower, bool* upper, real* interp, bool* output)
		{
			YELO_ASSERT_DISPLAY(lower, "Interpolation lower values pointer is null");
			YELO_ASSERT_DISPLAY(upper, "Interpolation upper values pointer is null");
			YELO_ASSERT_DISPLAY(interp, "Interpolation interp values pointer is null");
			YELO_ASSERT_DISPLAY(output, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			bool values[ValueCount];
			for (size_t i = 0; i < ValueCount; i++)
				values[i] = (interp[i] > 0.5 ? lower[i] : upper[i]);

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(bool)* ValueCount);
		}
		template<size_t ValueCount>
		static void InterpolateValues(int32* lower, int32* upper, real* interp, int32* output)
		{
			YELO_ASSERT_DISPLAY(lower, "Interpolation lower values pointer is null");
			YELO_ASSERT_DISPLAY(upper, "Interpolation upper values pointer is null");
			YELO_ASSERT_DISPLAY(interp, "Interpolation interp values pointer is null");
			YELO_ASSERT_DISPLAY(output, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			int32 values[ValueCount];
			for (size_t i = 0; i < ValueCount; i++)
			{
				real range = CAST(real, upper[i] - lower[i]);
				values[i] = CAST(int, (range * interp[i]) + lower[i]);
			}

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(int)* ValueCount);
		}
		template<size_t ValueCount>
		static void InterpolateValues(real* lower, real* upper, real* interp, real* output)
		{
			YELO_ASSERT_DISPLAY(lower, "Interpolation lower values pointer is null");
			YELO_ASSERT_DISPLAY(upper, "Interpolation upper values pointer is null");
			YELO_ASSERT_DISPLAY(interp, "Interpolation interp values pointer is null");
			YELO_ASSERT_DISPLAY(output, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			real values[ValueCount];
			for (size_t i = 0; i < ValueCount; i++)
			{
				real range = upper[i] - lower[i];
				values[i] = (range * interp[i]) + lower[i];
			}

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(real)* ValueCount);
		}
	};};
};