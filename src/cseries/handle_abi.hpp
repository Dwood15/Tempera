/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	template<typename T, size_t TSize>
	struct handle_abi_type_traits;

	template<typename T>
	struct handle_abi_type_traits<T, sizeof(sbyte)>
	{
		typedef sbyte bits_type;
	};
	template<typename T>
	struct handle_abi_type_traits<T, sizeof(short)>
	{
		typedef short bits_type;
	};
	template<typename T>
	struct handle_abi_type_traits<T, sizeof(long)>
	{
		typedef long bits_type;
	};
	template<typename T>
	struct handle_abi_type_traits<T, sizeof(__int64)>
	{
		typedef __int64 bits_type;
	};

	template<typename T> inline
	bool HandleIsNull(const T& handle)
	{
		typedef handle_abi_type_traits<T, sizeof(T)> type_traits;

		return *reinterpret_cast<const type_traits::bits_type *>(&handle) == NULL;
	}
	template<typename T> inline
	bool HandleIsNone(const T& handle)
	{
		typedef handle_abi_type_traits<T, sizeof(T)> type_traits;

		return *reinterpret_cast<const type_traits::bits_type *>(&handle) == NONE;
	}
};
