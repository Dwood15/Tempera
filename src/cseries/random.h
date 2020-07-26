#pragma once

#include "../math/real_math.h"
#include "../math/int_math.h"

namespace Yelo::Random {
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initializes the random number generator's seed. </summary>
	///
	/// <param name="seed">	The seed value. </param>
	void InitializeSeed(const int32 seed);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random real within the specified bounds. </summary>
	///
	/// <param name="lower">	The lower value. </param>
	/// <param name="upper">	The upper value. </param>
	///
	/// <returns>	A random real. </returns>
	real GetReal(const real lower, const real upper);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random real within the specified bounds. </summary>
	///
	/// <param name="bounds">	The value bounds. </param>
	///
	/// <returns>	A random real. </returns>
	real GetReal(const real_bounds bounds);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random real within the specified bounds. </summary>
	///
	/// <param name="bounds">	The value bounds. </param>
	///
	/// <returns>	A random real. </returns>
	real GetReal(const real_fraction_bounds bounds);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random angle within the specified bounds. </summary>
	///
	/// <param name="bounds">	The value bounds. </param>
	///
	/// <returns>	A random angle. </returns>
	real GetReal(const angle_bounds bounds);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random uint32 within the specified bounds. </summary>
	///
	/// <param name="lower">	The lower value. </param>
	/// <param name="upper">	The upper value. </param>
	///
	/// <returns>	A random uint32. </returns>
	uint32 GetUInt32(const uint32 lower, const uint32 upper);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random int32 within the specified bounds. </summary>
	///
	/// <param name="lower">	The lower value. </param>
	/// <param name="upper">	The upper value. </param>
	///
	/// <returns>	A random int32. </returns>
	int32 GetInt32(const int32 lower, const int32 upper);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random uint16 within the specified bounds. </summary>
	///
	/// <param name="lower">	The lower value. </param>
	/// <param name="upper">	The upper value. </param>
	///
	/// <returns>	A random uint16. </returns>
	uint16 GetUInt16(const uint16 lower, const uint16 upper);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random int16 within the specified bounds. </summary>
	///
	/// <param name="lower">	The lower value. </param>
	/// <param name="upper">	The upper value. </param>
	///
	/// <returns>	A random int16. </returns>
	int16 GetInt16(const int16 lower, const int16 upper);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a random int16 within the specified bounds. </summary>
	///
	/// <param name="bounds">	The value bounds. </param>
	///
	/// <returns>	A random int16. </returns>
	int16 GetInt16(const short_bounds bounds);
};
